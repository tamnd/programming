// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-size-of-a-file/.

#define _POSIX_C_SOURCE 200809L

#include "file_size.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static const char *dir = "file_size_tree";
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

static void expect(int ok, const char *what) {
    if (!ok) {
        fprintf(stderr, "%s\n", what);
        failures++;
    }
}

static void fresh(void) {
    if (system("rm -rf file_size_tree") != 0 || mkdir(dir, 0755) != 0) exit(2);
}

static void run_case(const char *name, const char *path, const char *data, size_t n) {
    char p[256];
    off_t size = -1;
    fresh();
    put(path, data, n);
    expect(file_size(at(p, path), &size) == 0 && size == (off_t)n, name);
}

static void test_errors(void) {
    char p[256];
    off_t size;
    fresh();
    errno = 0;
    expect(file_size(at(p, "a.txt"), &size) == -1 && errno == ENOENT, "missing");
    if (mkdir(at(p, "dir"), 0755) != 0) exit(2);
    errno = 0;
    expect(file_size(at(p, "dir"), &size) == -1 && errno == EISDIR, "directory");
}

static void test_link_sparse_open(void) {
    char p[256], q[256];
    off_t size = -1;
    fresh();
    put("a.txt", "alpha beta\n", 11);
    expect(symlink("a.txt", at(p, "link")) == 0, "symlink");
    expect(file_size(p, &size) == 0 && size == 11, "link");

    int f = open(at(p, "sparse.bin"), O_WRONLY | O_CREAT | O_EXCL, 0644);
    expect(f >= 0 && ftruncate(f, (off_t)5 << 30) == 0 && close(f) == 0, "make sparse");
    expect(file_size(p, &size) == 0 && size == 5368709120, "sparse");

    FILE *log = fopen(at(q, "app.log"), "w");
    if (log == NULL) exit(2);
    fputs("alpha\n", log);
    expect(file_size(q, &size) == 0 && size == 0, "before fflush");
    expect(fflush(log) == 0 && file_size(q, &size) == 0 && size == 6, "after fflush");
    fclose(log);
}

int main(void) {
    run_case("text", "a.txt", "alpha\n", 6);
    run_case("non-ascii-text", "a.txt", "caf\xc3\xa9\n", 6);
    run_case("empty-file", "a.txt", "", 0);
    run_case("binary-data", "a.bin", "\x00\r\n\xc3\xbf\x00", 6);
    run_case("crlf-line-ends", "a.txt", "a\r\nb\r\n", 6);
    run_case("name-with-spaces", "my notes.txt", "alpha\n", 6);
    run_case("non-ascii-name", "caf\xc3\xa9 \xe6\x97\xa5\xe6\x9c\xac.txt", "alpha\n", 6);
    test_errors();
    test_link_sparse_open();
    if (system("rm -rf file_size_tree") != 0) failures++;
    if (failures != 0) {
        fprintf(stderr, "%d failures\n", failures);
        return 1;
    }
    puts("ok");
    return 0;
}
