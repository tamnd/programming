// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-temporary-file/.

#define _POSIX_C_SOURCE 200809L

#include "temp_file.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static const char *dir = "temp_file_tree";
static int failures = 0;

static void expect(int ok, const char *what) {
    if (!ok) {
        fprintf(stderr, "%s\n", what);
        failures++;
    }
}

static void fresh(void) {
    if (system("rm -rf temp_file_tree") != 0 || mkdir(dir, 0755) != 0 || mkdir("temp_file_tree/sub", 0755) != 0)
        exit(2);
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

int main(void) {
    static const struct {
        const char *name, *rel, *prefix, *data;
    } cases[] = {
        {"one-line", ".", "tmp-", "alpha\n"},
        {"many-lines", ".", "report-", "alpha\nbeta\ngamma\n"},
        {"in-subdirectory", "sub", "tmp-", "alpha\n"},
        {"empty-prefix", ".", "", "alpha\n"},
        {"empty-data", ".", "tmp-", ""},
        {"non-ascii-data", ".", "tmp-", "caf\xc3\xa9 \xe6\x97\xa5\xe6\x9c\xac\n"},
        {"prefix-with-spaces", ".", "my temp ", "alpha\n"},
        {"non-ascii-prefix", ".", "caf\xc3\xa9-", "alpha\n"},
    };
    char d[256], path[256];
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        fresh();
        snprintf(d, sizeof d, "%s/%s", dir, cases[i].rel);
        char *got = use_temp_file(d, cases[i].prefix, cases[i].data);
        expect(got != NULL && strcmp(got, cases[i].data) == 0, cases[i].name);
        free(got);
        expect(count(dir) == 1 && count("temp_file_tree/sub") == 0, cases[i].name);
    }

    static const char *prefixes[] = {"tmp-", "", "my temp ", "caf\xc3\xa9-"};
    for (size_t i = 0; i < sizeof prefixes / sizeof prefixes[0]; i++) {
        fresh();
        struct stat st;
        char want[256];
        snprintf(want, sizeof want, "%s/%s", dir, prefixes[i]);
        int ok = make_temp_file(dir, prefixes[i], path, sizeof path) == 0;
        expect(ok && strncmp(path, want, strlen(want)) == 0 && strlen(path) >= strlen(want) + 6, "make: name");
        expect(ok && lstat(path, &st) == 0 && S_ISREG(st.st_mode) && (st.st_mode & 07777) == 0600 && st.st_size == 0,
               "make: mode and size");
    }

    fresh();
    char a[256], b[256];
    expect(make_temp_file(dir, "tmp-", a, sizeof a) == 0 && make_temp_file(dir, "tmp-", b, sizeof b) == 0 &&
               strcmp(a, b) != 0 && count(dir) == 3,
           "two calls give two files");

    fresh();
    errno = 0;
    expect(use_temp_file("temp_file_tree/nodir", "tmp-", "alpha\n") == NULL && errno == ENOENT, "missing directory");
    errno = 0;
    expect(use_temp_file(dir, "sub/tmp-", "alpha\n") == NULL && errno == EINVAL, "prefix with a slash");
    errno = 0;
    char small[8];
    expect(make_temp_file(dir, "tmp-", small, sizeof small) == -1 && errno == ERANGE, "path too long");
    expect(count("temp_file_tree/sub") == 0 && count(dir) == 1, "no file is left");

    if (system("rm -rf temp_file_tree") != 0) failures++;
    if (failures != 0) return 1;
    printf("ok\n");
    return 0;
}
