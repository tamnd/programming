// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-file-exists/.

// mkfifo, symlink and geteuid are in POSIX.1-2008, not in ISO C.
#define _POSIX_C_SOURCE 200809L

#include "file_status.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static const char *dir = "file_status_tree";
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

static void check(const char *path, const char *want) {
    const char *got = file_status(path);
    if (got == NULL || strcmp(got, want) != 0) {
        fprintf(stderr, "file_status(%s) = %s, want %s\n", path, got ? got : "NULL", want);
        failures++;
    }
}

int main(void) {
    if (system("rm -rf file_status_tree") != 0 || mkdir(dir, 0755) != 0) return 2;
    touch("a.txt", "alpha\n");
    touch("empty.txt", "");
    mkdir(at("dir"), 0755);
    check(at("a.txt"), "file");
    check(at("empty.txt"), "file");
    check(at("dir"), "directory");
    check(at("dir/"), "directory");
    check(at("missing.txt"), "missing");
    check(at("no-such-dir/a.txt"), "missing");
    check(at("a.txt/b.txt"), "missing");

    char link[256];
    snprintf(link, sizeof link, "%s", at("to-file"));
    symlink("a.txt", link);
    check(link, "file");
    snprintf(link, sizeof link, "%s", at("to-dir"));
    symlink("dir", link);
    check(link, "directory");
    snprintf(link, sizeof link, "%s", at("to-nothing"));
    symlink("missing.txt", link);
    check(link, "broken-link");

    if (access("/dev/null", F_OK) == 0) check("/dev/null", "other");
    snprintf(link, sizeof link, "%s", at("pipe"));
    if (mkfifo(link, 0644) != 0) return 2;
    check(link, "other");

    if (geteuid() != 0) {
        char locked[256];
        snprintf(locked, sizeof locked, "%s", at("locked"));
        mkdir(locked, 0755);
        touch("locked/a.txt", "alpha\n");
        chmod(locked, 0);
        errno = 0;
        const char *got = file_status(at("locked/a.txt"));
        if (got != NULL || errno != EACCES) {
            fprintf(stderr, "locked: got %s, errno %d, want NULL, EACCES\n", got ? got : "NULL", errno);
            failures++;
        }
        chmod(locked, 0700);
    }
    if (system("rm -rf file_status_tree") != 0) return 2;
    if (failures > 0) return 1;
    puts("ok");
    return 0;
}
