// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-file/.

#define _POSIX_C_SOURCE 200809L

#include "write_text.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define NON_ASCII "na\xc3\xafve caf\xc3\xa9\n\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e\n\xf0\x9f\x99\x82\n"

struct test_case {
    const char *name;
    const char *text;
    enum write_mode mode;
    const char *before; // NULL: no file before the call
    const char *want;
};

static const struct test_case cases[] = {
    {"new-file", "alpha\nbeta\n", WRITE_TRUNCATE, NULL, "alpha\nbeta\n"},
    {"append-existing", "two\n", WRITE_APPEND, "one\n", "one\ntwo\n"},
    {"atomic-replace", "new\n", WRITE_ATOMIC, "old\n", "new\n"},
    {"truncate-longer", "short\n", WRITE_TRUNCATE, "a much longer old line\n", "short\n"},
    {"empty-truncate", "", WRITE_TRUNCATE, "old\n", ""},
    {"empty-new", "", WRITE_TRUNCATE, NULL, ""},
    {"append-missing", "first\n", WRITE_APPEND, NULL, "first\n"},
    {"append-no-newline", "two", WRITE_APPEND, "one", "onetwo"},
    {"non-ascii", NON_ASCII, WRITE_TRUNCATE, NULL, NON_ASCII},
    {"crlf", "one\r\ntwo\r\n", WRITE_TRUNCATE, NULL, "one\r\ntwo\r\n"},
    {"mixed-endings", "lf\ncrlf\r\ncr\rend", WRITE_TRUNCATE, NULL, "lf\ncrlf\r\ncr\rend"},
    {"backslashes", "C:\\temp\\new\n", WRITE_TRUNCATE, NULL, "C:\\temp\\new\n"},
    {"format-chars", "-n 100% %s\n", WRITE_TRUNCATE, NULL, "-n 100% %s\n"},
    {"atomic-new", "new\n", WRITE_ATOMIC, NULL, "new\n"},
    {"atomic-empty", "", WRITE_ATOMIC, "old\n", ""},
    {"atomic-non-ascii", NON_ASCII, WRITE_ATOMIC, "old\n", NON_ASCII},
};

// The number of entries in dir, and the name of the last one.
static int count(const char *dir, char *last, size_t size) {
    DIR *d = opendir(dir);
    if (d == NULL) {
        return -1;
    }
    int n = 0;
    struct dirent *e;
    while ((e = readdir(d)) != NULL) {
        if (strcmp(e->d_name, ".") != 0 && strcmp(e->d_name, "..") != 0) {
            n++;
            snprintf(last, size, "%s", e->d_name);
        }
    }
    closedir(d);
    return n;
}

static int only_out(const char *dir) {
    char last[256] = "";
    return count(dir, last, sizeof last) == 1 && strcmp(last, "out.txt") == 0;
}

// Reads the file into buf. It returns the length, or -1.
static long slurp(const char *path, char *buf, size_t size) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return -1;
    }
    size_t n = fread(buf, 1, size, f);
    fclose(f);
    return (long)n;
}

static void cleanup(const char *dir, const char *path) {
    struct stat st;
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
        rmdir(path);
    } else {
        remove(path);
    }
    rmdir(dir);
}

// Makes a new empty directory with a name from the process id and a count.
static int make_dir(char *dir, size_t size) {
    static int n = 0;
    snprintf(dir, size, "/tmp/write-text-%ld-%d", (long)getpid(), n++);
    return mkdir(dir, 0700);
}

int main(void) {
    int failures = 0;
    char path[512];
    char buf[256];
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        char dir[64];
        if (make_dir(dir, sizeof dir) != 0) {
            perror("mkdir");
            return 1;
        }
        snprintf(path, sizeof path, "%s/out.txt", dir);
        if (c->before != NULL) {
            write_text(path, c->before, strlen(c->before), WRITE_TRUNCATE);
        }
        size_t wlen = strlen(c->want);
        if (write_text(path, c->text, strlen(c->text), c->mode) != 0) {
            printf("FAIL %s: %s\n", c->name, strerror(errno));
            failures++;
        } else if (slurp(path, buf, sizeof buf) != (long)wlen || memcmp(buf, c->want, wlen) != 0) {
            printf("FAIL %s: wrong bytes\n", c->name);
            failures++;
        } else if (!only_out(dir)) {
            printf("FAIL %s: other files in the directory\n", c->name);
            failures++;
        }
        cleanup(dir, path);
    }

    static const char *const names[] = {"truncate", "append", "atomic"};
    for (int m = WRITE_TRUNCATE; m <= WRITE_ATOMIC; m++) {
        char dir[64];
        if (make_dir(dir, sizeof dir) != 0) {
            perror("mkdir");
            return 1;
        }
        snprintf(path, sizeof path, "%s/missing/out.txt", dir);
        char last[256] = "";
        errno = 0;
        if (write_text(path, "x\n", 2, (enum write_mode)m) != -1 || errno != ENOENT) {
            printf("FAIL missing-dir-%s: no ENOENT\n", names[m]);
            failures++;
        } else if (count(dir, last, sizeof last) != 0) {
            printf("FAIL missing-dir-%s: files in the directory\n", names[m]);
            failures++;
        }
        rmdir(dir);
    }

    char dir[64];
    if (make_dir(dir, sizeof dir) != 0) {
        perror("mkdir");
        return 1;
    }
    snprintf(path, sizeof path, "%s/out.txt", dir);
    mkdir(path, 0777);
    struct stat st;
    if (write_text(path, "x\n", 2, WRITE_ATOMIC) != -1) {
        printf("FAIL atomic-onto-directory: no error\n");
        failures++;
    } else if (stat(path, &st) != 0 || !S_ISDIR(st.st_mode) || !only_out(dir)) {
        printf("FAIL atomic-onto-directory: the directory changed\n");
        failures++;
    }
    cleanup(dir, path);

    printf("%d failures\n", failures);
    return failures == 0 ? 0 : 1;
}
