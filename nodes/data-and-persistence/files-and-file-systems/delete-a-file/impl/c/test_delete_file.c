// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/delete-a-file/.

// symlink, lstat and geteuid are in POSIX.1-2008, not in ISO C.
#define _POSIX_C_SOURCE 200809L

#include "delete_file.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static const char *dir = "delete_file_tree";
static int failures = 0;

static const char *at(const char *name) {
    static char path[256];
    snprintf(path, sizeof path, "%s/%s", dir, name);
    return path;
}

static void touch(const char *name, const char *text) {
    FILE *f = fopen(at(name), "w");
    if (f == NULL || fputs(text, f) < 0 || fclose(f) != 0) exit(2);
}

static int lexists(const char *path) {
    struct stat st;
    return lstat(path, &st) == 0;
}

static void check(const char *name, int got, int want, const char *path) {
    if (got != want || lexists(path)) {
        fprintf(stderr, "%s: delete_file = %d, want %d, entry there: %d\n", name, got, want,
                lexists(path));
        failures++;
    }
}

int main(void) {
    if (system("rm -rf delete_file_tree") != 0 || mkdir(dir, 0755) != 0) return 2;

    // The cases of cases/basic.jsonl and cases/edge.jsonl.
    touch("a.txt", "alpha\n");
    check("delete", delete_file(at("a.txt")), 1, at("a.txt"));
    check("missing", delete_file(at("a.txt")), 0, at("a.txt"));
    touch("a.txt", "");
    check("empty-file", delete_file(at("a.txt")), 1, at("a.txt"));
    check("missing-parent", delete_file(at("no-such-dir/a.txt")), 0, at("no-such-dir/a.txt"));
    touch("my notes.txt", "alpha\n");
    check("name-with-spaces", delete_file(at("my notes.txt")), 1, at("my notes.txt"));
    touch("café 日本.txt", "alpha\n");
    check("non-ascii-name", delete_file(at("café 日本.txt")), 1, at("café 日本.txt"));

    // A path through a regular file gives ENOTDIR, and no entry is there.
    touch("a.txt", "alpha\n");
    check("through-a-file", delete_file(at("a.txt/b.txt")), 0, at("a.txt/b.txt"));

    // A directory is an error, and it stays.
    if (mkdir(at("dir"), 0755) != 0) return 2;
    if (delete_file(at("dir")) != -1 || !lexists(at("dir"))) {
        fprintf(stderr, "directory: no error\n");
        failures++;
    }

    // A link is deleted, not its target.
    if (symlink("a.txt", at("to-file")) != 0 || symlink("dir", at("to-dir")) != 0 ||
        symlink("missing.txt", at("broken")) != 0)
        return 2;
    check("to-file", delete_file(at("to-file")), 1, at("to-file"));
    check("to-dir", delete_file(at("to-dir")), 1, at("to-dir"));
    check("broken", delete_file(at("broken")), 1, at("broken"));
    if (!lexists(at("a.txt")) || !lexists(at("dir"))) {
        fprintf(stderr, "links: a target is gone\n");
        failures++;
    }

    // A read-only file is deleted: the permissions of the directory decide.
    if (chmod(at("a.txt"), 0444) != 0) return 2;
    check("read-only", delete_file(at("a.txt")), 1, at("a.txt"));

    // A file in a directory with no write permission gives EACCES and stays.
    if (geteuid() != 0) {
        if (mkdir(at("locked"), 0755) != 0) return 2;
        touch("locked/a.txt", "alpha\n");
        if (chmod(at("locked"), 0500) != 0) return 2;
        static char locked_file[256];
        snprintf(locked_file, sizeof locked_file, "%s/locked/a.txt", dir);
        if (delete_file(locked_file) != -1 || errno != EACCES || !lexists(locked_file)) {
            fprintf(stderr, "locked: no EACCES\n");
            failures++;
        }
        if (chmod(at("locked"), 0700) != 0) return 2;
    }

    // An open file is deleted, and the open stream can still read it.
    touch("open.txt", "alpha\n");
    FILE *f = fopen(at("open.txt"), "r");
    if (f == NULL) return 2;
    check("open", delete_file(at("open.txt")), 1, at("open.txt"));
    char buf[16] = {0};
    if (fgets(buf, sizeof buf, f) == NULL || strcmp(buf, "alpha\n") != 0) {
        fprintf(stderr, "open: cannot read the data\n");
        failures++;
    }
    fclose(f);

    if (system("rm -rf delete_file_tree") != 0) return 2;
    if (failures == 0) printf("ok\n");
    return failures == 0 ? 0 : 1;
}
