// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-with-buffers/.

// access is in POSIX.1-2008, not in ISO C.
#define _POSIX_C_SOURCE 200809L

#include "copy_buffered.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const char *src = "copy_buffered_in.bin";
static const char *dst = "copy_buffered_out.bin";

static void write_file(const char *path, const unsigned char *data, size_t n) {
    FILE *f = fopen(path, "wb");
    assert(f != NULL);
    assert(fwrite(data, 1, n, f) == n);
    assert(fclose(f) == 0);
}

static unsigned char *pattern(size_t n) {
    unsigned char *b = malloc(n > 0 ? n : 1);
    assert(b != NULL);
    for (size_t i = 0; i < n; i++) b[i] = (unsigned char)i;
    return b;
}

static void check(const char *name, const unsigned char *data, size_t n, long size) {
    write_file(src, data, n);
    long long got = copy_buffered(src, dst, size);
    if (got != (long long)n) {
        fprintf(stderr, "case %s: copy_buffered = %lld, want %zu\n", name, got, n);
        exit(1);
    }
    FILE *f = fopen(dst, "rb");
    assert(f != NULL);
    unsigned char *back = malloc(n + 1);
    assert(back != NULL);
    size_t m = fread(back, 1, n + 1, f);
    fclose(f);
    if (m != n || memcmp(back, data, n) != 0) {
        fprintf(stderr, "case %s: dst is not the same as src\n", name);
        exit(1);
    }
    free(back);
}

static void check_error(const char *name, const char *from, const char *to, long size, int want) {
    errno = 0;
    long long got = copy_buffered(from, to, size);
    if (got != -1 || errno != want) {
        fprintf(stderr, "case %s: copy_buffered = %lld, errno %d, want -1, errno %d\n", name, got, errno, want);
        exit(1);
    }
}

int main(void) {
    const unsigned char *text = (const unsigned char *)"alpha\nbeta\ngamma\n";
    unsigned char *all = pattern(256);
    unsigned char *large = pattern(1 << 20);
    check("text", text, 17, 4096);
    check("several-buffers", text, 17, 4);
    check("empty", (const unsigned char *)"", 0, 8);
    check("smaller-than-buffer", (const unsigned char *)"12345", 5, 8);
    check("exact-buffer", (const unsigned char *)"12345678", 8, 8);
    check("one-past-buffer", (const unsigned char *)"123456789", 9, 8);
    check("size-one", text, 17, 1);
    check("binary", all, 256, 64);
    check("large", large, 1 << 20, 4096);
    check("large-odd-buffer", large, 1 << 20, 4093);

    write_file(dst, (const unsigned char *)"old and longer", 14);
    check("replaces-a-longer-file", (const unsigned char *)"new", 3, 8);

    remove(dst);
    check_error("missing-src", "copy_buffered_missing.bin", dst, 8, ENOENT);
    assert(access(dst, F_OK) != 0);
    write_file(src, text, 17);
    check_error("missing-dst-dir", src, "copy_buffered_no_such_dir/out.bin", 8, ENOENT);
    check_error("zero-size", src, dst, 0, EINVAL);
    check_error("negative-size", src, dst, -1, EINVAL);

    if (access("/dev/full", F_OK) == 0) {
        // 17 bytes stay in the buffer, so the error comes only from fclose.
        check_error("full-device-small", src, "/dev/full", 4096, ENOSPC);
        write_file(src, large, 1 << 20);
        check_error("full-device-large", src, "/dev/full", 4096, ENOSPC);
    }
    remove(src);
    remove(dst);
    free(all);
    free(large);
    puts("ok");
    return 0;
}
