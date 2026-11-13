// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/truncate-a-file/.

#define _POSIX_C_SOURCE 200809L

#include "truncate_file.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static const char *dir = "truncate_file_tree";
static int failures = 0;

static char *at(char *buf, const char *name) {
    snprintf(buf, 256, "%s/%s", dir, name);
    return buf;
}

static void put(const char *name, const char *data, size_t n) {
    char p[256];
    FILE *f = fopen(at(p, name), "wb");
    if (f == NULL || fwrite(data, 1, n, f) != n || fclose(f) != 0) exit(2);
}

static int same(const char *name, const char *want, size_t n) {
    char p[256], data[64];
    FILE *f = fopen(at(p, name), "rb");
    if (f == NULL) return 0;
    size_t got = fread(data, 1, sizeof data, f);
    fclose(f);
    return got == n && memcmp(data, want, n) == 0;
}

static void expect(int ok, const char *what) {
    if (!ok) {
        fprintf(stderr, "%s\n", what);
        failures++;
    }
}

static void fresh(void) {
    if (system("rm -rf truncate_file_tree") != 0 || mkdir(dir, 0755) != 0) exit(2);
}

static void run_case(const char *name, const char *path, off_t length, const char *before, size_t nb,
                     const char *after, size_t na) {
    char p[256];
    fresh();
    put(path, before, nb);
    expect(truncate_file(at(p, path), length) == 0, name);
    expect(same(path, after, na), name);
}

static void test_errors(void) {
    char p[256];
    struct stat st;
    fresh();
    errno = 0;
    expect(truncate_file(at(p, "a.txt"), 0) == -1 && errno == ENOENT, "missing");
    expect(lstat(at(p, "a.txt"), &st) != 0, "no file made");
    if (mkdir(at(p, "dir"), 0755) != 0) exit(2);
    errno = 0;
    expect(truncate_file(at(p, "dir"), 0) == -1 && errno == EISDIR, "directory");
    put("a.txt", "alpha\n", 6);
    errno = 0;
    expect(truncate_file(at(p, "a.txt"), -1) == -1 && errno == EINVAL, "negative");
    expect(same("a.txt", "alpha\n", 6), "no change");
}

static void test_open(void) {
    char p[256];
    fresh();
    put("app.log", "alpha beta\n", 11);
    int log = open(at(p, "app.log"), O_WRONLY | O_APPEND);
    expect(log >= 0 && truncate_file(p, 0) == 0 && write(log, "new\n", 4) == 4, "append");
    close(log);
    expect(same("app.log", "new\n", 4), "append data");
    put("data.bin", "alpha beta\n", 11);
    int f = open(at(p, "data.bin"), O_WRONLY);
    expect(f >= 0 && lseek(f, 0, SEEK_END) == 11, "seek");
    expect(truncate_file(p, 5) == 0 && write(f, "X", 1) == 1, "hole");
    close(f);
    expect(same("data.bin", "alpha\0\0\0\0\0\0X", 12), "hole data");
}

int main(void) {
    run_case("shrink", "a.txt", 5, "alpha beta\n", 11, "alpha", 5);
    run_case("extend", "a.txt", 8, "alpha", 5, "alpha\0\0\0", 8);
    run_case("same-length", "a.txt", 6, "alpha\n", 6, "alpha\n", 6);
    run_case("to-zero", "a.txt", 0, "alpha\n", 6, "", 0);
    run_case("empty-file", "a.txt", 3, "", 0, "\0\0\0", 3);
    run_case("name-with-spaces", "my notes.txt", 2, "alpha\n", 6, "al", 2);
    test_errors();
    test_open();
    if (system("rm -rf truncate_file_tree") != 0) failures++;
    if (failures != 0) {
        fprintf(stderr, "%d failures\n", failures);
        return 1;
    }
    puts("ok");
    return 0;
}
