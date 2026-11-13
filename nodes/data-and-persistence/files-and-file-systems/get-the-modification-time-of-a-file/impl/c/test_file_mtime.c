// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-modification-time-of-a-file/.

#define _POSIX_C_SOURCE 200809L

#include "file_mtime.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

static const char *dir = "file_mtime_tree";
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
    if (system("rm -rf file_mtime_tree") != 0 || mkdir(dir, 0755) != 0) exit(2);
}

static void make(const char *name) {
    char p[256];
    FILE *f = fopen(at(p, name), "w");
    if (f == NULL || fputs("alpha\n", f) < 0 || fclose(f) != 0) exit(2);
}

// set_mtime sets the modification time of path to sec + nsec / 1e9 seconds.
static void set_mtime(const char *path, time_t sec, long nsec) {
    struct timespec ts[2] = {{0, UTIME_OMIT}, {sec, nsec}};
    if (utimensat(AT_FDCWD, path, ts, 0) != 0) exit(2);
}

static void run_case(const char *name, const char *rel, time_t sec, long nsec, time_t want) {
    char p[256];
    time_t got = 12345;
    fresh();
    if (rel[0] == 'd') {
        if (mkdir(at(p, rel), 0755) != 0) exit(2);
    } else {
        make(rel);
    }
    set_mtime(at(p, rel), sec, nsec);
    expect(file_mtime(p, &got) == 0 && got == want, name);
}

static void test_errors(void) {
    char p[256];
    time_t got;
    fresh();
    errno = 0;
    expect(file_mtime(at(p, "a.txt"), &got) == -1 && errno == ENOENT, "missing");
}

static void test_link_and_write(void) {
    char p[256], q[256];
    time_t got = 0;
    fresh();
    make("a.txt");
    set_mtime(at(p, "a.txt"), 1709296496, 0);
    expect(symlink("a.txt", at(q, "link")) == 0, "symlink");
    expect(file_mtime(q, &got) == 0 && got == 1709296496, "link");

    set_mtime(p, 0, 0);
    time_t before = time(NULL) - 1;
    int f = open(p, O_WRONLY | O_APPEND);
    expect(f >= 0 && write(f, "beta\n", 5) == 5 && close(f) == 0, "write");
    expect(file_mtime(p, &got) == 0 && got >= before, "present time");
}

int main(void) {
    // A time of -1.5 s is -2 s and 500000000 ns: tv_nsec is never negative.
    run_case("whole-second", "a.txt", 1709296496, 0, 1709296496);
    run_case("sub-second", "a.txt", 1709296496, 500000000, 1709296496);
    run_case("epoch", "a.txt", 0, 0, 0);
    run_case("before-1970", "a.txt", -86400, 0, -86400);
    run_case("before-1970-sub-second", "a.txt", -2, 500000000, -2);
    run_case("after-2038", "a.txt", (time_t)4102444800, 0, (time_t)4102444800);
    run_case("directory", "dir", 1709296496, 0, 1709296496);
    run_case("name-with-spaces", "my notes.txt", 1709296496, 0, 1709296496);
    test_errors();
    test_link_and_write();
    if (system("rm -rf file_mtime_tree") != 0) failures++;
    if (failures != 0) {
        fprintf(stderr, "%d failures\n", failures);
        return 1;
    }
    puts("ok");
    return 0;
}
