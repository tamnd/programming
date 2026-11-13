// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/set-file-permissions/.

#define _POSIX_C_SOURCE 200809L

#include "set_mode.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

static const char *dir = "set_mode_tree";
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

static void fresh(void) {
    if (system("chmod -R u+rwx set_mode_tree 2>/dev/null; rm -rf set_mode_tree") != 0 ||
        mkdir(dir, 0755) != 0)
        exit(2);
}

static void new_file(const char *name) {
    char p[256];
    FILE *f = fopen(at(p, name), "wb");
    if (f == NULL || fputs("alpha\n", f) < 0 || fclose(f) != 0 || chmod(p, 0644) != 0) exit(2);
}

static mode_t mode_of(const char *name) {
    char p[256];
    struct stat st;
    if (stat(at(p, name), &st) != 0) exit(2);
    return st.st_mode & 07777;
}

int main(void) {
    static const struct {
        const char *name, *rel, *mode;
        mode_t result;
    } cases[] = {
        {"owner-read-write-group-read", "a.txt", "640", 0640},
        {"owner-only", "a.txt", "600", 0600},
        {"executable", "a.txt", "755", 0755},
        {"same-mode", "a.txt", "644", 0644},
        {"no-permissions", "a.txt", "000", 0},
        {"leading-zero", "a.txt", "0640", 0640},
        {"set-user-id", "a.txt", "4755", 04755},
        {"name-with-spaces", "my notes.txt", "640", 0640},
        {"non-ascii-name", "caf\xc3\xa9 \xe6\x97\xa5\xe6\x9c\xac.txt", "640", 0640},
    };
    char p[256];
    mode_t got;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        fresh();
        new_file(cases[i].rel);
        int rc = set_mode(at(p, cases[i].rel), cases[i].mode, &got);
        expect(rc == 0 && got == cases[i].result, cases[i].name);
        expect(mode_of(cases[i].rel) == cases[i].result, cases[i].name);
    }

    fresh();
    new_file("a.txt");
    const char *invalid[] = {"648", "64", "10640", "u+x", "", "640\n", "+640", " 640"};
    for (size_t i = 0; i < sizeof invalid / sizeof invalid[0]; i++) {
        errno = 0;
        expect(set_mode(at(p, "a.txt"), invalid[i], &got) == -1 && errno == EINVAL, invalid[i]);
    }
    expect(mode_of("a.txt") == 0644, "invalid modes change nothing");

    errno = 0;
    expect(set_mode(at(p, "missing.txt"), "640", &got) == -1 && errno == ENOENT, "missing");

    fresh();
    new_file("a.txt");
    if (symlink("a.txt", at(p, "link")) != 0) exit(2);
    expect(set_mode(p, "600", &got) == 0 && got == 0600, "link");
    expect(mode_of("a.txt") == 0600, "link changes the target");

    fresh();
    if (mkdir(at(p, "dir"), 0755) != 0) exit(2);
    expect(set_mode(p, "700", &got) == 0 && got == 0700, "directory");

    fresh();
    new_file("a.txt");
    mode_t old = umask(077);
    expect(set_mode(at(p, "a.txt"), "666", &got) == 0 && got == 0666, "umask does not apply");
    umask(old);

    fresh();
    rmdir(dir);
    if (failures == 0) printf("ok\n");
    return failures == 0 ? 0 : 1;
}
