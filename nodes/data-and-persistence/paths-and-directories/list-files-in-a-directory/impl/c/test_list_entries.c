// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/list-files-in-a-directory/.

#define _XOPEN_SOURCE 700

#include "list_entries.h"

#include <errno.h>
#include <fcntl.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX 8

struct test_case {
    const char *name;
    const char *tree[MAX];
    bool recursive;
    const char *want[MAX];
};

static const struct test_case cases[] = {
    {"flat", {"b.txt", "a.txt", "c.txt"}, false, {"a.txt", "b.txt", "c.txt"}},
    {"nested",
     {"docs/", "docs/guide.md", "docs/img/", "docs/img/logo.png", "readme.md"},
     true,
     {"docs/", "docs/guide.md", "docs/img/", "docs/img/logo.png", "readme.md"}},
    {"nested-shallow",
     {"docs/", "docs/guide.md", "docs/img/", "docs/img/logo.png", "readme.md"},
     false,
     {"docs/", "readme.md"}},
    {"empty", {0}, true, {0}},
    {"empty-subdir", {"empty/"}, true, {"empty/"}},
    {"hidden", {".env", ".git/", ".git/config", "main.go"}, true, {".env", ".git/", ".git/config", "main.go"}},
    {"case-order", {"a.txt", "B.txt", "c.txt"}, false, {"B.txt", "a.txt", "c.txt"}},
    {"dot-before-slash", {"a/", "a/x", "a.txt", "a-b"}, true, {"a-b", "a.txt", "a/", "a/x"}},
    {"symlink-to-dir", {"real/", "real/f", "link -> real"}, true, {"link", "real/", "real/f"}},
    {"symlink-to-file", {"f", "g -> f"}, true, {"f", "g"}},
    {"broken-symlink", {"dangling -> nowhere"}, true, {"dangling"}},
    {"symlink-loop", {"d/", "d/up -> .."}, true, {"d/", "d/up"}},
    {"spaces", {"my file.txt", "two  spaces/", "two  spaces/x"}, true, {"my file.txt", "two  spaces/", "two  spaces/x"}},
    {"non-ascii", {"\xc3\xa9t\xc3\xa9.txt", "zebra.txt", "apple.txt"}, false, {"apple.txt", "zebra.txt", "\xc3\xa9t\xc3\xa9.txt"}},
};

static const char *root = "list_entries_tree";

static int remove_one(const char *path, const struct stat *st, int flag, struct FTW *ftw) {
    (void)st;
    (void)flag;
    (void)ftw;
    return remove(path);
}

static void make_tree(const char *const *tree) {
    nftw(root, remove_one, 16, FTW_DEPTH | FTW_PHYS);
    if (mkdir(root, 0755) != 0) {
        perror("mkdir");
        exit(1);
    }
    char path[256];
    for (size_t i = 0; i < MAX && tree[i] != NULL; i++) {
        const char *entry = tree[i];
        const char *arrow = strstr(entry, " -> ");
        int rc;
        if (arrow != NULL) {
            snprintf(path, sizeof path, "%s/%.*s", root, (int)(arrow - entry), entry);
            rc = symlink(arrow + 4, path);
        } else {
            snprintf(path, sizeof path, "%s/%s", root, entry);
            size_t len = strlen(entry);
            if (entry[len - 1] == '/') {
                rc = mkdir(path, 0755);
            } else {
                int fd = open(path, O_WRONLY | O_CREAT | O_EXCL, 0644);
                rc = fd < 0 ? -1 : close(fd);
            }
        }
        if (rc != 0) {
            perror(path);
            exit(1);
        }
    }
}

int main(void) {
    int failures = 0;
    for (size_t c = 0; c < sizeof cases / sizeof cases[0]; c++) {
        const struct test_case *tc = &cases[c];
        make_tree(tc->tree);
        char **got;
        size_t n;
        if (list_entries(root, tc->recursive, &got, &n) != 0) {
            fprintf(stderr, "case %s: %s\n", tc->name, strerror(errno));
            failures++;
            continue;
        }
        size_t want_n = 0;
        while (want_n < MAX && tc->want[want_n] != NULL) want_n++;
        bool ok = n == want_n;
        for (size_t i = 0; ok && i < n; i++) ok = strcmp(got[i], tc->want[i]) == 0;
        if (!ok) {
            fprintf(stderr, "case %s: got", tc->name);
            for (size_t i = 0; i < n; i++) fprintf(stderr, " [%s]", got[i]);
            fprintf(stderr, "\n");
            failures++;
        }
        free_entries(got, n);
    }

    make_tree((const char *const[]){"notes.txt", NULL});
    char **got;
    size_t n;
    errno = 0;
    if (list_entries("list_entries_tree/missing", true, &got, &n) != -1 || errno != ENOENT) {
        fprintf(stderr, "case missing: no ENOENT\n");
        failures++;
    }
    errno = 0;
    if (list_entries("list_entries_tree/notes.txt", true, &got, &n) != -1 || errno != ENOTDIR) {
        fprintf(stderr, "case not-a-directory: no ENOTDIR\n");
        failures++;
    }
    nftw(root, remove_one, 16, FTW_DEPTH | FTW_PHYS);
    if (failures > 0) return 1;
    puts("ok");
    return 0;
}
