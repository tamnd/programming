// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rename-a-file/.

// symlink, readlink, lstat and getpid are in POSIX.1-2008, not in ISO C.
#define _POSIX_C_SOURCE 200809L

#include "rename_file.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static const char *dir = "rename_file_tree";
static int failures = 0;

static char *at(char *buf, const char *name) {
    snprintf(buf, 256, "%s/%s", dir, name);
    return buf;
}

static void put(const char *name, const char *text) {
    char p[256];
    FILE *f = fopen(at(p, name), "w");
    if (f == NULL || fputs(text, f) < 0 || fclose(f) != 0) exit(2);
}

static const char *get(const char *name) {
    static char data[64];
    char p[256];
    FILE *f = fopen(at(p, name), "r");
    if (f == NULL) return "(none)";
    size_t n = fread(data, 1, sizeof data - 1, f);
    data[n] = '\0';
    fclose(f);
    return data;
}

static int lexists(const char *name) {
    char p[256];
    struct stat st;
    return lstat(at(p, name), &st) == 0;
}

static void expect(int ok, const char *what) {
    if (!ok) {
        fprintf(stderr, "%s\n", what);
        failures++;
    }
}

static void run_case(const char *name, const char *src, const char *dst, const char *src_text,
                     const char *dst_text, const char *want) {
    char s[256], d[256];
    if (system("rm -rf rename_file_tree") != 0 || mkdir(dir, 0755) != 0) exit(2);
    if (strchr(dst, '/') != NULL && mkdir(at(d, "sub"), 0755) != 0) exit(2);
    if (dst_text != NULL) put(dst, dst_text);
    put(src, src_text);
    if (rename_file(at(s, src), at(d, dst)) != 0 || strcmp(get(dst), want) != 0 ||
        (strcmp(src, dst) != 0 && lexists(src))) {
        fprintf(stderr, "%s: failed\n", name);
        failures++;
    }
}

int main(void) {
    // The cases of cases/basic.jsonl and cases/edge.jsonl.
    run_case("rename", "a.txt", "b.txt", "alpha\n", NULL, "alpha\n");
    run_case("replace-existing", "a.txt", "b.txt", "alpha\n", "beta\n", "alpha\n");
    run_case("same-path", "a.txt", "a.txt", "alpha\n", "alpha\n", "alpha\n");
    run_case("empty-file", "a.txt", "b.txt", "", "beta\n", "");
    run_case("other-directory", "a.txt", "sub/a.txt", "alpha\n", NULL, "alpha\n");
    run_case("name-with-spaces", "my notes.txt", "old notes.txt", "alpha\n", NULL, "alpha\n");
    run_case("non-ascii-name", "a.txt", "café 日本.txt", "alpha\n", NULL, "alpha\n");

    char s[256], d[256];
    if (system("rm -rf rename_file_tree") != 0 || mkdir(dir, 0755) != 0) return 2;
    expect(rename_file(at(s, "a.txt"), at(d, "b.txt")) == -1 && errno == ENOENT, "missing src: no ENOENT");
    put("a.txt", "alpha\n");
    expect(rename_file(at(s, "a.txt"), at(d, "no-such-dir/b.txt")) == -1 && errno == ENOENT,
           "missing dst dir: no ENOENT");
    if (mkdir(at(d, "dir"), 0755) != 0) return 2;
    expect(rename_file(at(s, "a.txt"), at(d, "dir")) == -1 && errno == EISDIR, "dst is a directory: no EISDIR");
    expect(strcmp(get("a.txt"), "alpha\n") == 0, "errors: src is changed");

    // A link is renamed, not its target.
    if (symlink("a.txt", at(s, "link")) != 0) return 2;
    expect(rename_file(at(s, "link"), at(d, "new-link")) == 0 && !lexists("link"), "link: not renamed");
    char target[64] = {0};
    expect(readlink(at(d, "new-link"), target, sizeof target - 1) == 5 && strcmp(target, "a.txt") == 0,
           "link: wrong target");

    // A program that has the old dst open still reads the old data.
    put("b.txt", "beta\n");
    FILE *old = fopen(at(d, "b.txt"), "r");
    if (old == NULL) return 2;
    expect(rename_file(at(s, "a.txt"), at(d, "b.txt")) == 0, "open: rename failed");
    char buf[16] = {0};
    expect(fgets(buf, sizeof buf, old) != NULL && strcmp(buf, "beta\n") == 0, "open: old data is gone");
    fclose(old);
    expect(strcmp(get("b.txt"), "alpha\n") == 0, "open: dst is not the new file");

    // On Linux, /dev/shm is often another file system.
    struct stat shm, tmp;
    if (stat("/dev/shm", &shm) == 0 && stat(dir, &tmp) == 0 && shm.st_dev != tmp.st_dev) {
        char dst[64];
        snprintf(dst, sizeof dst, "/dev/shm/rename-file-%ld.txt", (long)getpid());
        expect(rename_file(at(s, "b.txt"), dst) == -1 && errno == EXDEV, "cross device: no EXDEV");
        expect(lexists("b.txt") && access(dst, F_OK) != 0, "cross device: a file is changed");
    }

    if (system("rm -rf rename_file_tree") != 0) return 2;
    if (failures == 0) printf("ok\n");
    return failures == 0 ? 0 : 1;
}
