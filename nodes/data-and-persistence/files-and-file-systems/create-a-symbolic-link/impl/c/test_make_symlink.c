// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-symbolic-link/.

#define _POSIX_C_SOURCE 200809L

#include "make_symlink.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static const char *dir = "make_symlink_tree";
static int failures = 0;

static char *at(char *buf, const char *name) {
    snprintf(buf, 256, "%s/%s", dir, name);
    return buf;
}

static void expect(int ok, const char *what) {
    if (!ok) {
        fprintf(stderr, "%s\n", what);
        failures++;
    }
}

static void write_file(const char *name, const char *data) {
    char p[256];
    FILE *f = fopen(at(p, name), "wb");
    if (f == NULL || fputs(data, f) < 0 || fclose(f) != 0) exit(2);
}

static void fresh(void) {
    char p[256];
    if (system("rm -rf make_symlink_tree") != 0 || mkdir(dir, 0755) != 0 ||
        mkdir(at(p, "sub"), 0755) != 0)
        exit(2);
    write_file("a.txt", "alpha\n");
    write_file("sub/b.txt", "beta\n");
}

int main(void) {
    static const struct {
        const char *name, *target, *link;
    } cases[] = {
        {"file-target", "a.txt", "link"},
        {"directory-target", "sub", "link"},
        {"link-in-subdirectory", "b.txt", "sub/link"},
        {"dangling-target", "missing.txt", "link"},
        {"relative-target", "a.txt", "sub/link"},
        {"parent-target", "../a.txt", "sub/link"},
        {"name-with-spaces", "my notes.txt", "my link"},
        {"non-ascii-name", "caf\xc3\xa9 \xe6\x97\xa5\xe6\x9c\xac.txt", "lien \xe6\x97\xa5\xe6\x9c\xac"},
    };
    char p[256], buf[256];
    struct stat st;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        fresh();
        ssize_t n = make_symlink(cases[i].target, at(p, cases[i].link), buf, sizeof buf);
        expect(n == (ssize_t)strlen(cases[i].target) && strcmp(buf, cases[i].target) == 0,
               cases[i].name);
        expect(lstat(p, &st) == 0 && S_ISLNK(st.st_mode), cases[i].name);
    }

    fresh();
    errno = 0;
    expect(make_symlink("b.txt", at(p, "a.txt"), buf, sizeof buf) == -1 && errno == EEXIST, "file exists");
    errno = 0;
    expect(make_symlink("a.txt", at(p, "sub"), buf, sizeof buf) == -1 && errno == EEXIST, "directory exists");

    if (symlink("missing.txt", at(p, "dangling")) != 0) exit(2);
    errno = 0;
    expect(make_symlink("a.txt", p, buf, sizeof buf) == -1 && errno == EEXIST, "dangling link exists");

    errno = 0;
    expect(make_symlink("a.txt", at(p, "nodir/link"), buf, sizeof buf) == -1 && errno == ENOENT, "missing directory");

    char small[5];
    errno = 0;
    expect(make_symlink("long-target.txt", at(p, "small"), small, sizeof small) == -1 && errno == ERANGE, "small buffer");

    expect(make_symlink("../a.txt", at(p, "sub/up"), buf, sizeof buf) == 8, "parent link");
    FILE *f = fopen(p, "rb");
    char data[16] = {0};
    expect(f != NULL && fgets(data, sizeof data, f) != NULL && strcmp(data, "alpha\n") == 0, "link reads target");
    if (f != NULL) fclose(f);

    if (system("rm -rf make_symlink_tree") != 0) return 2;
    if (failures == 0) printf("ok\n");
    return failures == 0 ? 0 : 1;
}
