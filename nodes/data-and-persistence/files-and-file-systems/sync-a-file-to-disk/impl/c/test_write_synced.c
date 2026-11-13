// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sync-a-file-to-disk/.

#define _POSIX_C_SOURCE 200809L

#include "write_synced.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define NON_ASCII "na\xc3\xafve caf\xc3\xa9\n\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e\n\xf0\x9f\x99\x82\n"

static const char *dir = "sync_tree";
static const char *path = "sync_tree/out.txt";
static int failures = 0;

static void expect(int ok, const char *what) {
    if (!ok) {
        fprintf(stderr, "%s\n", what);
        failures++;
    }
}

static void fresh(void) {
    if (system("rm -rf sync_tree && mkdir sync_tree") != 0) exit(2);
}

// count gives the number of entries in d, without . and ..
static int count(const char *d) {
    DIR *dp = opendir(d);
    if (dp == NULL) exit(2);
    int n = 0;
    struct dirent *e;
    while ((e = readdir(dp)) != NULL)
        if (strcmp(e->d_name, ".") != 0 && strcmp(e->d_name, "..") != 0) n++;
    closedir(dp);
    return n;
}

// equal checks that the file at p holds the bytes of want.
static int equal(const char *p, const char *want) {
    FILE *f = fopen(p, "rb");
    if (f == NULL) return 0;
    char buf[256];
    size_t n = fread(buf, 1, sizeof buf, f);
    fclose(f);
    return n == strlen(want) && memcmp(buf, want, n) == 0;
}

int main(void) {
    static const struct {
        const char *name, *text, *before;
    } cases[] = {
        {"new-file", "alpha\nbeta\n", NULL},
        {"replace-longer", "short\n", "a much longer old line\n"},
        {"many-lines", "one\ntwo\nthree\nfour\n", NULL},
        {"empty-replace", "", "old\n"},
        {"empty-new", "", NULL},
        {"no-final-newline", "last line", NULL},
        {"non-ascii", NON_ASCII, NULL},
        {"crlf", "one\r\ntwo\r\n", NULL},
    };
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        fresh();
        if (cases[i].before != NULL) {
            FILE *f = fopen(path, "wb");
            if (f == NULL) return 2;
            fputs(cases[i].before, f);
            fclose(f);
        }
        int rc = write_synced(path, cases[i].text, strlen(cases[i].text));
        expect(rc == 0 && equal(path, cases[i].text), cases[i].name);
        expect(count(dir) == 1, cases[i].name);
    }

    fresh();
    expect(sync_directory(dir) == 0, "sync-directory");

    fresh();
    errno = 0;
    expect(write_synced("sync_tree/missing/out.txt", "x\n", 2) == -1 && errno == ENOENT, "missing-directory");
    expect(count(dir) == 0, "missing-directory: no file");

    fresh();
    mkdir(path, 0755);
    errno = 0;
    expect(write_synced(path, "x\n", 2) == -1 && errno == EISDIR, "onto-directory");
    struct stat st;
    expect(stat(path, &st) == 0 && S_ISDIR(st.st_mode), "onto-directory: the directory stays");

    if (system("rm -rf sync_tree") != 0) return 2;
    if (failures == 0) printf("all tests pass\n");
    return failures == 0 ? 0 : 1;
}
