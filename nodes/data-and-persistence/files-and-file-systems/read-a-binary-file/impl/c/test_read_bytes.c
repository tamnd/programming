// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-binary-file/.

// mkfifo, fork and waitpid are in POSIX.1-2008, not in ISO C.
#define _POSIX_C_SOURCE 200809L

#include "read_bytes.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static const char *path = "read_bytes_input.bin";

static void write_file(const unsigned char *data, size_t n) {
    FILE *f = fopen(path, "wb");
    assert(f != NULL);
    assert(fwrite(data, 1, n, f) == n);
    assert(fclose(f) == 0);
}

static void check(const char *name, const unsigned char *data, size_t n) {
    write_file(data, n);
    size_t len = 99;
    unsigned char *got = read_bytes(path, &len);
    if (got == NULL || len != n || memcmp(got, data, n) != 0) {
        fprintf(stderr, "case %s: wrong bytes\n", name);
        exit(1);
    }
    free(got);
}

static void check_exact(const char *name, const unsigned char *data, size_t size, size_t n) {
    write_file(data, size);
    unsigned char *buf = malloc(n > 0 ? n : 1);
    assert(buf != NULL);
    if (read_exact(path, buf, n) != READ_EXACT_OK || memcmp(buf, data, n) != 0) {
        fprintf(stderr, "case %s: wrong bytes\n", name);
        exit(1);
    }
    free(buf);
}

static void check_short(const char *name, const unsigned char *data, size_t size, size_t n) {
    write_file(data, size);
    unsigned char buf[32];
    assert(n <= sizeof buf);
    if (read_exact(path, buf, n) != READ_EXACT_EOF) {
        fprintf(stderr, "case %s: want READ_EXACT_EOF\n", name);
        exit(1);
    }
}

static void check_error(const char *name, const char *file, int want) {
    size_t len;
    unsigned char buf[1];
    errno = 0;
    if (read_bytes(file, &len) != NULL || errno != want) {
        fprintf(stderr, "case %s: read_bytes: want errno %d, got %d\n", name, want, errno);
        exit(1);
    }
    errno = 0;
    if (read_exact(file, buf, 1) != READ_EXACT_ERROR || errno != want) {
        fprintf(stderr, "case %s: read_exact: want errno %d, got %d\n", name, want, errno);
        exit(1);
    }
}

// A pipe gives the bytes in two parts, so one read call gets only the first
// part. read_exact must read again to get all 8 bytes.
static void check_reads_again(void) {
    const char *fifo = "read_bytes_fifo";
    remove(fifo);
    assert(mkfifo(fifo, 0600) == 0);
    pid_t pid = fork();
    assert(pid >= 0);
    if (pid == 0) {
        FILE *out = fopen(fifo, "wb");
        if (out == NULL) _exit(1);
        setvbuf(out, NULL, _IONBF, 0);
        fwrite("abc", 1, 3, out);
        nanosleep(&(struct timespec){.tv_nsec = 200000000}, NULL);
        fwrite("defgh", 1, 5, out);
        _exit(fclose(out) == 0 ? 0 : 1);
    }
    unsigned char buf[8];
    enum read_exact_result r = read_exact(fifo, buf, 8);
    int status;
    assert(waitpid(pid, &status, 0) == pid && WIFEXITED(status) && WEXITSTATUS(status) == 0);
    remove(fifo);
    if (r != READ_EXACT_OK || memcmp(buf, "abcdefgh", 8) != 0) {
        fprintf(stderr, "case reads-again: wrong bytes\n");
        exit(1);
    }
}

#define BYTES(...) ((const unsigned char[]){__VA_ARGS__})
#define CASE(name, ...) check(name, BYTES(__VA_ARGS__), sizeof(BYTES(__VA_ARGS__)))

int main(void) {
    static const unsigned char signature[] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a,
                                              0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52};
    unsigned char all[256];
    for (int i = 0; i < 256; i++) all[i] = (unsigned char)i;
    static const unsigned char zeros[8] = {0};
    static const unsigned char ctrl_z[] = {'a', 0x1a, 'b'};

    check("signature", signature, sizeof signature);
    CASE("bytes", 0x00, 0x01, 0x02, 0x7f, 0x80, 0xfe, 0xff);
    check("empty", all, 0);
    check("all-bytes", all, sizeof all);
    check("zeros", zeros, sizeof zeros);
    CASE("crlf", 'a', '\r', '\n', 'b', '\r', '\n');
    check("ctrl-z", ctrl_z, sizeof ctrl_z);
    CASE("not-utf8", 0xc3, 0x28, 0xff, 0xfe, 0x80);

    size_t big = 1 << 20;
    unsigned char *large = malloc(big);
    assert(large != NULL);
    for (size_t i = 0; i < big; i++) large[i] = (unsigned char)i;
    check("large", large, big);

    check_exact("signature-only", signature, sizeof signature, 8);
    check_exact("first-four", all, sizeof all, 4);
    check_exact("whole-file", zeros, sizeof zeros, 8);
    check_exact("zero-count", all, 0, 0);
    check_exact("across-ctrl-z", ctrl_z, sizeof ctrl_z, 3);
    check_exact("large", large, big, big);
    free(large);

    check_short("one-past-end", signature, sizeof signature, 17);
    check_short("empty-file", all, 0, 1);

    remove(path);
    check_error("missing", "no_such_file.bin", ENOENT);
    check_error("directory", ".", EISDIR);
    check_reads_again();

    puts("all tests passed");
    return 0;
}
