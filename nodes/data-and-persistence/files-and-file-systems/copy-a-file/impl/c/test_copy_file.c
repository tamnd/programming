// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-a-file/.

// symlink, link, lstat and getpid are in POSIX.1-2008, not in ISO C.
#define _POSIX_C_SOURCE 200809L

#include "copy_file.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static const char *dir = "copy_file_tree";
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

// get reads the file into a buffer and returns its size, or -1.
static long get(const char *name, char *data, size_t cap) {
    char p[256];
    FILE *f = fopen(at(p, name), "rb");
    if (f == NULL) return -1;
    size_t n = fread(data, 1, cap, f);
    fclose(f);
    return (long)n;
}

static void expect(int ok, const char *what) {
    if (!ok) {
        fprintf(stderr, "%s\n", what);
        failures++;
    }
}

static void fresh(void) {
    if (system("rm -rf copy_file_tree") != 0 || mkdir(dir, 0755) != 0) exit(2);
}

static int same(const char *name, const char *want, size_t n) {
    char data[64];
    return get(name, data, sizeof data) == (long)n && memcmp(data, want, n) == 0;
}

static void run_case(const char *name, const char *src, const char *dst, const char *src_text,
                     size_t n, const char *dst_text) {
    char s[256], d[256];
    fresh();
    if (strchr(dst, '/') != NULL && mkdir(at(d, "sub"), 0755) != 0) exit(2);
    if (dst_text != NULL) put(dst, dst_text, strlen(dst_text));
    put(src, src_text, n);
    expect(copy_file(at(s, src), at(d, dst)) == 0, name);
    expect(same(dst, src_text, n), name);
    expect(same(src, src_text, n), name);
}

static void test_errors(void) {
    char s[256], d[256];
    fresh();
    errno = 0;
    expect(copy_file(at(s, "a.txt"), at(d, "b.txt")) == -1 && errno == ENOENT, "missing src");
    put("a.txt", "alpha\n", 6);
    errno = 0;
    expect(copy_file(at(s, "a.txt"), at(d, "no-such-dir/b.txt")) == -1 && errno == ENOENT,
           "missing dst dir");
    if (mkdir(at(d, "dir"), 0755) != 0) exit(2);
    errno = 0;
    expect(copy_file(at(s, "dir"), at(d, "b.txt")) == -1 && errno == EISDIR, "src dir");
    errno = 0;
    expect(copy_file(at(s, "a.txt"), at(d, "dir")) == -1 && errno == EISDIR, "dst dir");
    struct stat st;
    expect(lstat(at(d, "b.txt"), &st) != 0, "no b.txt");
    if (link(at(s, "a.txt"), at(d, "c.txt")) != 0) exit(2);
    errno = 0;
    expect(copy_file(at(s, "a.txt"), at(d, "a.txt")) == -1 && errno == EINVAL, "same path");
    errno = 0;
    expect(copy_file(at(s, "a.txt"), at(d, "c.txt")) == -1 && errno == EINVAL, "hard link");
    expect(same("a.txt", "alpha\n", 6), "same file keeps its data");
}

static void test_mode(void) {
    char s[256], d[256];
    struct stat st;
    fresh();
    put("a.sh", "echo alpha\n", 11);
    put("c.sh", "beta\n", 5);
    if (chmod(at(s, "a.sh"), 0750) != 0 || chmod(at(d, "c.sh"), 0640) != 0) exit(2);
    expect(copy_file(at(s, "a.sh"), at(d, "b.sh")) == 0, "mode new");
    expect(stat(at(d, "b.sh"), &st) == 0 && (st.st_mode & 07777) == 0750, "mode new bits");
    expect(copy_file(at(s, "a.sh"), at(d, "c.sh")) == 0, "mode replace");
    expect(stat(at(d, "c.sh"), &st) == 0 && (st.st_mode & 07777) == 0750, "mode replace bits");
}

static void test_large(void) {
    enum { N = 5000003 };
    char s[256], d[256];
    char *data = malloc(N), *got = malloc(N + 1);
    if (data == NULL || got == NULL) exit(2);
    for (long i = 0; i < N; i++) data[i] = (char)(i * 7 % 251);
    fresh();
    put("big.bin", data, N);
    expect(copy_file(at(s, "big.bin"), at(d, "copy.bin")) == 0, "large");
    expect(get("copy.bin", got, N + 1) == N && memcmp(got, data, N) == 0, "large data");
    free(data);
    free(got);
}

static void test_link_at_src(void) {
    char s[256], d[256];
    struct stat st;
    fresh();
    put("target.txt", "alpha\n", 6);
    if (symlink("target.txt", at(s, "link.txt")) != 0) exit(2);
    expect(copy_file(at(s, "link.txt"), at(d, "b.txt")) == 0, "link");
    expect(lstat(at(d, "b.txt"), &st) == 0 && S_ISREG(st.st_mode), "link gives a file");
    expect(same("b.txt", "alpha\n", 6), "link data");
}

int main(void) {
    run_case("copy", "a.txt", "b.txt", "alpha\n", 6, NULL);
    run_case("replace-existing", "a.txt", "b.txt", "alpha\n", 6, "beta, a longer text\n");
    run_case("empty-file", "a.txt", "b.txt", "", 0, "beta\n");
    run_case("other-directory", "a.txt", "sub/a.txt", "alpha\n", 6, NULL);
    run_case("name-with-spaces", "my notes.txt", "copy of my notes.txt", "alpha\n", 6, NULL);
    run_case("non-ascii-name", "a.txt", "caf\xc3\xa9 \xe6\x97\xa5\xe6\x9c\xac.txt", "alpha\n", 6,
             NULL);
    run_case("binary-data", "a.bin", "b.bin", "\0\r\n\xc3\xbf\0", 6, NULL);
    test_errors();
    test_mode();
    test_large();
    test_link_at_src();
    if (system("rm -rf copy_file_tree") != 0) failures++;
    if (failures != 0) {
        fprintf(stderr, "%d failures\n", failures);
        return 1;
    }
    puts("ok");
    return 0;
}
