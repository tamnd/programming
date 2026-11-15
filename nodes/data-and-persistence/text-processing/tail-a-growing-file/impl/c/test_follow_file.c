// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/tail-a-growing-file/.

#define _POSIX_C_SOURCE 200809L

#include "follow_file.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static int failures = 0;

#define CHECK(ok, name)                                          \
    do {                                                         \
        if (!(ok)) {                                             \
            fprintf(stderr, "FAIL %s: line %d\n", name, __LINE__); \
            failures++;                                          \
        }                                                        \
    } while (0)

// Each poll is one string, with an LF after each line.
struct test_case {
    const char *name, *initial;
    const char *steps[9];
    const char *want[4];
};

static const struct test_case cases[] = {
    {"append-lines", "old\n", {"append:a\nb\n", "poll", "append:c\n", "poll", NULL}, {"a\nb\n", "c\n", NULL}},
    {"partial-line", "", {"append:ab", "poll", "append:c\nd", "poll", "append:\n", "poll", NULL}, {"", "abc\n", "d\n", NULL}},
    {"rotate", "1\n", {"append:2\n", "poll", "rotate", "append:3\n", "poll", NULL}, {"2\n", "3\n", NULL}},
    {"truncate", "", {"append:a\nb\n", "poll", "truncate:", "poll", "append:c\n", "poll", NULL}, {"a\nb\n", "", "c\n", NULL}},
    {"no-change", "x\n", {"poll", "poll", NULL}, {"", "", NULL}},
    {"empty-line", "", {"append:\n\na\n", "poll", NULL}, {"\n\na\n", NULL}},
    {"crlf", "", {"append:a\r\nb\r\n", "poll", NULL}, {"a\r\nb\r\n", NULL}},
    {"truncate-and-write", "", {"append:aaaa\n", "poll", "truncate:b\n", "poll", NULL}, {"aaaa\n", "b\n", NULL}},
    {"truncate-partial", "", {"append:abc", "poll", "truncate:d\n", "poll", NULL}, {"", "abc\nd\n", NULL}},
    {"rotate-late-writes", "", {"append:1\n", "poll", "rotate", "append-old:2\n", "append:3\n", "poll", NULL}, {"1\n", "2\n3\n", NULL}},
    {"rotate-partial", "", {"append:a", "poll", "rotate", "append:b\n", "poll", NULL}, {"", "a\nb\n", NULL}},
    {"rotate-twice", "", {"append:1\n", "poll", "rotate", "append:2\n", "poll", "rotate", "append:3\n", "poll", NULL}, {"1\n", "2\n", "3\n", NULL}},
    {"delete", "", {"append:1\n", "poll", "delete", "poll", "poll", NULL}, {"1\n", "", "", NULL}},
    {"delete-partial", "", {"append:x", "poll", "delete", "poll", NULL}, {"", "x\n", NULL}},
    {"delete-and-create", "", {"append:1\n", "poll", "delete", "poll", "create:2\n", "poll", NULL}, {"1\n", "", "2\n", NULL}},
    {"delete-and-create-between-polls", "", {"append:1\n", "poll", "delete", "create:2\n", "poll", NULL}, {"1\n", "2\n", NULL}},
    {"start-mid-line", "abc", {"append:d\ne\n", "poll", NULL}, {"d\ne\n", NULL}},
    {"non-ascii", "", {"append:caf\303\251\nna\303\257ve\n", "poll", NULL}, {"caf\303\251\nna\303\257ve\n", NULL}},
};

static char dir[64], path[128], old[128];

static void free_polls(char **polls, size_t n) {
    for (size_t i = 0; i < n; i++) {
        free(polls[i]);
    }
    free(polls);
}

static void remove_files(void) {
    unlink(path);
    unlink(old);
}

// The lines and the polls of the follow test.
struct counts {
    int lines, polls;
};

static int count_line(const char *line, size_t len, void *arg) {
    (void)line;
    (void)len;
    ((struct counts *)arg)->lines++;
    return 0;
}

// Stop after 2 lines, or after 500 polls (5 seconds) if a line is lost.
static int two_lines(void *arg) {
    struct counts *c = arg;
    return c->lines >= 2 || ++c->polls > 500;
}

int main(void) {
    // mkdtemp is not visible on all systems with _POSIX_C_SOURCE, so the test
    // makes its own directory.
    snprintf(dir, sizeof dir, "/tmp/follow-file-%ld", (long)getpid());
    if (mkdir(dir, 0700) != 0) {
        perror(dir);
        return 1;
    }
    snprintf(path, sizeof path, "%s/followed.txt", dir);
    snprintf(old, sizeof old, "%s/followed.txt.1", dir);
    char **polls;
    size_t n;

    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        int r = follow_file(path, c->initial, c->steps, &polls, &n);
        CHECK(r == 0, c->name);
        if (r == 0) {
            size_t want = 0;
            while (c->want[want] != NULL) {
                want++;
            }
            CHECK(n == want, c->name);
            for (size_t j = 0; j < n && j < want; j++) {
                CHECK(strcmp(polls[j], c->want[j]) == 0, c->name);
            }
            free_polls(polls, n);
        }
        remove_files();
    }

    // not-found: no directory, so the first write fails.
    char missing[160];
    snprintf(missing, sizeof missing, "%s/missing/followed.txt", dir);
    const char *poll[] = {"poll", NULL};
    errno = 0;
    CHECK(follow_file(missing, "", poll, &polls, &n) == -1 && errno == ENOENT, "not-found");
    struct follower f;
    errno = 0;
    CHECK(follower_open(&f, missing) == -1 && errno == ENOENT, "not-found");

    // 100000 lines in one poll.
    size_t size = 0;
    char *text = malloc(700000);
    size += (size_t)snprintf(text, 8, "append:");
    for (int i = 0; i < 100000; i++) {
        size += (size_t)snprintf(text + size, 16, "%d\n", i);
    }
    const char *many[] = {text, "poll", NULL};
    CHECK(follow_file(path, "", many, &polls, &n) == 0, "many-lines");
    if (n == 1) {
        size_t lines = 0;
        for (const char *s = polls[0]; *s != '\0'; s++) {
            lines += *s == '\n';
        }
        CHECK(lines == 100000, "many-lines");
        CHECK(strncmp(polls[0], "0\n1\n", 4) == 0, "many-lines");
        CHECK(strcmp(polls[0] + strlen(polls[0]) - 6, "99999\n") == 0, "many-lines");
        free_polls(polls, n);
    }
    free(text);
    remove_files();

    // A character across two polls.
    const char *split[] = {"append:caf\303", "poll", "append:\251\n", "poll", NULL};
    CHECK(follow_file(path, "", split, &polls, &n) == 0, "character-across-polls");
    if (n == 2) {
        CHECK(strcmp(polls[0], "") == 0 && strcmp(polls[1], "caf\303\251\n") == 0, "character-across-polls");
        free_polls(polls, n);
    }
    remove_files();

    // follower_follow: the lines that come after the follower starts. The
    // lines are written first, so that the test ends on its own.
    const char *start[] = {"append:old\n", NULL};
    CHECK(follow_file(path, "", start, &polls, &n) == 0, "follow");
    free_polls(polls, n);
    CHECK(follower_open(&f, path) == 0, "follow");
    FILE *w = fopen(path, "ab");
    fputs("a\nb\n", w);
    fclose(w);
    struct counts count = {0, 0};
    CHECK(follower_follow(&f, 10, two_lines, count_line, &count) == 0 && count.lines == 2, "follow");
    follower_close(&f);
    remove_files();

    rmdir(dir);
    if (failures > 0) {
        return 1;
    }
    printf("all tests pass\n");
    return 0;
}
