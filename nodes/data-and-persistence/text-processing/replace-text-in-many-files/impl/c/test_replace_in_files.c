// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-text-in-many-files/.

#define _POSIX_C_SOURCE 200809L

#include "replace_in_files.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static int failures = 0;

#define CHECK(cond, name)                                  \
    do {                                                   \
        if (!(cond)) {                                     \
            fprintf(stderr, "FAIL %s: %s\n", name, #cond); \
            failures++;                                    \
        }                                                  \
    } while (0)

static char dir[64];

struct file {
    const char *name, *before, *after;
};

struct test_case {
    const char *name, *old_text, *new_text;
    size_t n;
    struct file files[2];
    size_t want[2];
};

static void write_text(const char *path, const char *text) {
    FILE *f = fopen(path, "wb");
    fputs(text, f);
    fclose(f);
}

// Returns the text of the file in a new buffer that the caller frees.
static char *read_text(const char *path) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return NULL;
    }
    char *buf = calloc(1 << 21, 1);
    size_t n = fread(buf, 1, (1 << 21) - 1, f);
    buf[n] = '\0';
    fclose(f);
    return buf;
}

static int count_files(void) {
    DIR *d = opendir(dir);
    int n = 0;
    struct dirent *e;
    while ((e = readdir(d)) != NULL) {
        n += strcmp(e->d_name, ".") != 0 && strcmp(e->d_name, "..") != 0;
    }
    closedir(d);
    return n;
}

static void remove_files(void) {
    DIR *d = opendir(dir);
    struct dirent *e;
    char path[sizeof dir + sizeof e->d_name];
    while ((e = readdir(d)) != NULL) {
        if (strcmp(e->d_name, ".") != 0 && strcmp(e->d_name, "..") != 0) {
            snprintf(path, sizeof path, "%s/%s", dir, e->d_name);
            unlink(path);
        }
    }
    closedir(d);
}

static const struct test_case cases[] = {
    {"rosetta", "Goodbye London!", "Hello New York!", 2,
     {{"a.txt", "Goodbye London!\n", "Hello New York!\n"},
      {"b.txt", "Goodbye London! Goodbye London!\nGoodbye\n", "Hello New York! Hello New York!\nGoodbye\n"}},
     {1, 2}},
    {"some-files-match", "dog", "fox", 2, {{"a.txt", "cat\n", "cat\n"}, {"b.txt", "dog\ncat dog\n", "fox\ncat fox\n"}}, {0, 2}},
    {"one-file", "1", "2", 1, {{"a.txt", "x = 1\ny = 1\n", "x = 2\ny = 2\n"}}, {2}},
    {"no-files", "a", "b", 0, {{0}}, {0}},
    {"no-match", "x", "y", 1, {{"a.txt", "abc\n", "abc\n"}}, {0}},
    {"empty-file", "a", "b", 1, {{"a.txt", "", ""}}, {0}},
    {"delete", "-", "", 1, {{"a.txt", "a-b-c\n", "abc\n"}}, {2}},
    {"whole-file", "abc", "", 1, {{"a.txt", "abc", ""}}, {1}},
    {"new-contains-old", "a", "aa", 1, {{"a.txt", "a a\n", "aa aa\n"}}, {2}},
    {"overlapping", "aa", "b", 1, {{"a.txt", "aaa\n", "ba\n"}}, {1}},
    {"adjacent", "aa", "b", 1, {{"a.txt", "aaaa\n", "bb\n"}}, {2}},
    {"special-chars", "1.5", "\\&$1 $& [x]", 1, {{"a.txt", "1.5 and 125\n", "\\&$1 $& [x] and 125\n"}}, {1}},
    {"glob-chars", "*.txt", "?", 1, {{"a.txt", "a.txt *.txt\n", "a.txt ?\n"}}, {1}},
    {"across-lines", "end\nstart", "end start", 1, {{"a.txt", "end\nstart\n", "end start\n"}}, {1}},
    {"lf-to-crlf", "\n", "\r\n", 1, {{"a.txt", "a\nb\n", "a\r\nb\r\n"}}, {2}},
    {"crlf", "a", "c", 1, {{"a.txt", "a\r\nb\r\n", "c\r\nb\r\n"}}, {1}},
    {"no-final-newline", "b", "c", 1, {{"a.txt", "a b", "a c"}}, {1}},
    {"non-ascii", "\xc3\xa9", "e", 1, {{"a.txt", "caf\xc3\xa9" ", cafe\n", "cafe, cafe\n"}}, {1}},
    {"case-sensitive", "hello", "bye", 1, {{"a.txt", "Hello hello\n", "Hello bye\n"}}, {1}},
};

int main(void) {
    // mkdtemp is not visible on all systems with _POSIX_C_SOURCE, so the test
    // makes its own directory.
    snprintf(dir, sizeof dir, "/tmp/replace-in-files-%ld", (long)getpid());
    if (mkdir(dir, 0700) != 0) {
        perror(dir);
        return 1;
    }
    char path[2][128];
    const char *paths[3];
    size_t counts[3];

    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        for (size_t j = 0; j < c->n; j++) {
            snprintf(path[j], sizeof path[j], "%s/%s", dir, c->files[j].name);
            write_text(path[j], c->files[j].before);
            paths[j] = path[j];
        }
        int r = replace_in_files(paths, c->n, c->old_text, c->new_text, counts);
        CHECK(r == 0, c->name);
        for (size_t j = 0; j < c->n; j++) {
            CHECK(counts[j] == c->want[j], c->name);
            char *got = read_text(path[j]);
            CHECK(got != NULL && strcmp(got, c->files[j].after) == 0, c->name);
            free(got);
        }
        // No temporary file stays in the directory.
        CHECK(count_files() == (int)c->n, c->name);
        remove_files();
    }

    // 100000 lines: "line 0" to "line 99999" become "L0" to "L99999".
    snprintf(path[0], sizeof path[0], "%s/many.txt", dir);
    paths[0] = path[0];
    FILE *f = fopen(path[0], "wb");
    for (int i = 0; i < 100000; i++) {
        fprintf(f, "line %d\n", i);
    }
    fclose(f);
    CHECK(replace_in_files(paths, 1, "line ", "L", counts) == 0 && counts[0] == 100000, "many-lines");
    char *got = read_text(path[0]);
    char *want = calloc(1 << 21, 1);
    for (int i = 0, n = 0; i < 100000; i++) {
        n += snprintf(want + n, (size_t)((1 << 21) - n), "L%d\n", i);
    }
    CHECK(got != NULL && strcmp(got, want) == 0, "many-lines");
    free(want);
    free(got);
    remove_files();

    // The permissions stay, and a file with no instance is not written.
    snprintf(path[0], sizeof path[0], "%s/a.txt", dir);
    write_text(path[0], "a\n");
    chmod(path[0], 0640);
    CHECK(replace_in_files(paths, 1, "a", "b", counts) == 0 && counts[0] == 1, "keeps-the-permissions");
    struct stat st;
    CHECK(stat(path[0], &st) == 0 && (st.st_mode & 0777) == 0640, "keeps-the-permissions");
    ino_t before = st.st_ino;
    CHECK(replace_in_files(paths, 1, "x", "y", counts) == 0 && counts[0] == 0, "no-change");
    CHECK(stat(path[0], &st) == 0 && st.st_ino == before, "no-change");

    // Errors.
    errno = 0;
    CHECK(replace_in_files(paths, 1, "", "b", counts) == -1 && errno == EINVAL, "empty-old");
    got = read_text(path[0]);
    CHECK(got != NULL && strcmp(got, "b\n") == 0, "empty-old");
    free(got);
    remove_files();
    snprintf(path[0], sizeof path[0], "%s/missing.txt", dir);
    errno = 0;
    CHECK(replace_in_files(paths, 1, "a", "b", counts) == -1 && errno == ENOENT, "not-found");

    // The first error stops the function.
    char a[128], missing[128], c[128];
    snprintf(a, sizeof a, "%s/a.txt", dir);
    snprintf(missing, sizeof missing, "%s/missing.txt", dir);
    snprintf(c, sizeof c, "%s/c.txt", dir);
    write_text(a, "a\n");
    write_text(c, "a\n");
    const char *stop[] = {a, missing, c};
    CHECK(replace_in_files(stop, 3, "a", "b", counts) == -1 && errno == ENOENT, "stop");
    got = read_text(a);
    CHECK(got != NULL && strcmp(got, "b\n") == 0, "stop");
    free(got);
    got = read_text(c);
    CHECK(got != NULL && strcmp(got, "a\n") == 0, "stop");
    free(got);
    remove_files();
    rmdir(dir);

    if (failures == 0) {
        puts("ok");
    }
    return failures != 0;
}
