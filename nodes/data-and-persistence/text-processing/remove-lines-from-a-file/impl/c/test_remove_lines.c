// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-lines-from-a-file/.

#define _POSIX_C_SOURCE 200809L

#include "remove_lines.h"

#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static int failures = 0;

#define CHECK(cond, name)                                         \
    do {                                                          \
        if (!(cond)) {                                            \
            fprintf(stderr, "FAIL %s: %s\n", (name), #cond);      \
            failures++;                                           \
        }                                                         \
    } while (0)

static char dir[64];
static char path[128];

static void write_file(const char *data, size_t len) {
    FILE *f = fopen(path, "wb");
    if (f == NULL || fwrite(data, 1, len, f) != len || fclose(f) != 0) {
        perror("write_file");
        exit(1);
    }
}

// Reads the file into a buffer that the caller frees, and sets *len.
static char *read_file(size_t *len) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        perror("read_file");
        exit(1);
    }
    size_t cap = 1 << 20, n = 0;
    char *buf = malloc(cap);
    size_t got;
    while (buf != NULL && (got = fread(buf + n, 1, cap - n, f)) > 0) {
        n += got;
        if (n == cap) {
            cap *= 2;
            char *grown = realloc(buf, cap);
            if (grown == NULL) {
                free(buf);
                buf = NULL;
            } else {
                buf = grown;
            }
        }
    }
    fclose(f);
    if (buf == NULL) {
        exit(1);
    }
    *len = n;
    return buf;
}

// Counts the files in dir, without . and ..
static int files_in_dir(void) {
    DIR *d = opendir(dir);
    int n = 0;
    struct dirent *e;
    while (d != NULL && (e = readdir(d)) != NULL) {
        n += strcmp(e->d_name, ".") != 0 && strcmp(e->d_name, "..") != 0;
    }
    if (d != NULL) {
        closedir(d);
    }
    return n;
}

struct test_case {
    const char *name;
    const char *text;
    int start, count, want;
    const char *after;
};

static void check(const char *name, const char *text, size_t text_len, int start, int count,
                  int want, const char *after, size_t after_len) {
    write_file(text, text_len);
    int got = remove_lines(path, start, count);
    CHECK(got == want, name);
    size_t len;
    char *buf = read_file(&len);
    CHECK(len == after_len && memcmp(buf, after, len) == 0, name);
    free(buf);
    // No temporary file stays in the directory.
    CHECK(files_in_dir() == 1, name);
}

int main(void) {
    snprintf(dir, sizeof dir, "/tmp/remove-lines-%ld", (long)getpid());
    if (mkdir(dir, 0700) != 0) {
        perror("mkdir");
        return 1;
    }
    snprintf(path, sizeof path, "%s/input.txt", dir);
    static const struct test_case cases[] = {
        {"first-two", "a\nb\nc\nd\n", 1, 2, 2, "c\nd\n"},
        {"middle", "a\nb\nc\nd\n", 2, 2, 2, "a\nd\n"},
        {"last", "a\nb\nc\n", 3, 1, 1, "a\nb\n"},
        {"past-the-end", "a\nb\n", 3, 1, 0, "a\nb\n"},
        {"partly-past-the-end", "a\nb\nc\n", 2, 5, 2, "a\n"},
        {"count-zero", "a\nb\n", 1, 0, 0, "a\nb\n"},
        {"all-lines", "a\nb\n", 1, 2, 2, ""},
        {"empty", "", 1, 1, 0, ""},
        {"empty-line", "a\n\nb\n", 2, 1, 1, "a\nb\n"},
        {"no-final-newline", "a\nb", 1, 1, 1, "b"},
        {"remove-incomplete-line", "a\nb", 2, 1, 1, "a\n"},
        {"crlf", "a\r\nb\r\nc\r\n", 2, 1, 1, "a\r\nc\r\n"},
        {"lone-cr", "a\rb\nc\n", 1, 1, 1, "c\n"},
        {"non-ascii", "caf\xc3\xa9\nna\xc3\xafve\nzo\xc3\xab\n", 2, 1, 1, "caf\xc3\xa9\nzo\xc3\xab\n"},
        {"large-count", "a\nb\n", 2, INT_MAX, 1, "a\n"},
    };
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        check(c->name, c->text, strlen(c->text), c->start, c->count, c->want, c->after,
              strlen(c->after));
    }

    // 100000 lines with the numbers 0 to 99999. Lines 2 to 99999 go.
    size_t cap = 700000, n = 0;
    char *many = malloc(cap);
    if (many == NULL) {
        return 1;
    }
    for (int i = 0; i < 100000; i++) {
        n += (size_t)snprintf(many + n, cap - n, "%d\n", i);
    }
    check("many-lines", many, n, 2, 99998, 99998, "0\n99999\n", 8);
    free(many);

    // The file keeps its permissions.
    write_file("a\nb\n", 4);
    CHECK(chmod(path, 0640) == 0, "keeps-the-permissions");
    CHECK(remove_lines(path, 1, 1) == 1, "keeps-the-permissions");
    struct stat st;
    CHECK(stat(path, &st) == 0 && (st.st_mode & 0777) == 0640, "keeps-the-permissions");

    // If no line goes, the file is not written again.
    ino_t before = st.st_ino;
    CHECK(remove_lines(path, 5, 1) == 0, "no-change");
    CHECK(stat(path, &st) == 0 && st.st_ino == before, "no-change");

    // Errors.
    char missing[80];
    snprintf(missing, sizeof missing, "%s/missing.txt", dir);
    errno = 0;
    CHECK(remove_lines(missing, 1, 1) == -1 && errno == ENOENT, "not-found");
    write_file("a\nb\n", 4);
    errno = 0;
    CHECK(remove_lines(path, 0, 1) == -1 && errno == EINVAL, "start-zero");
    errno = 0;
    CHECK(remove_lines(path, 1, -1) == -1 && errno == EINVAL, "negative-count");
    size_t len;
    char *buf = read_file(&len);
    CHECK(len == 4 && memcmp(buf, "a\nb\n", 4) == 0, "bad-range");
    free(buf);

    unlink(path);
    rmdir(dir);
    if (failures == 0) {
        puts("ok");
    }
    return failures == 0 ? 0 : 1;
}
