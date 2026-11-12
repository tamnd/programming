// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/encode-and-decode-utf-8/.

#include "utf8.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void check_encode(const char *name, uint32_t cp, const unsigned char *want, size_t n) {
    unsigned char out[4];
    size_t k = encode_utf8(cp, out);
    if (k != n || memcmp(out, want, n) != 0) {
        fprintf(stderr, "encode %s: got %zu bytes, want %zu\n", name, k, n);
        failures++;
    }
}

static void check_decode(const char *name, const unsigned char *b, size_t n, long want) {
    long got = decode_utf8(b, n);
    if (got != want) {
        fprintf(stderr, "decode %s: got %ld, want %ld\n", name, got, want);
        failures++;
    }
}

// Each byte is a number from 0 to 255. A decode result of -1 means that the bytes are not one well-formed sequence.
int main(void) {
    check_encode("ascii-a", 0x41, (const unsigned char[]){65}, 1);
    check_encode("euro", 0x20AC, (const unsigned char[]){226, 130, 172}, 3);
    check_encode("emoji", 0x1F642, (const unsigned char[]){240, 159, 153, 130}, 4);
    check_encode("nul", 0x0, (const unsigned char[]){0}, 1);
    check_encode("one-byte-max", 0x7F, (const unsigned char[]){127}, 1);
    check_encode("two-byte-min", 0x80, (const unsigned char[]){194, 128}, 2);
    check_encode("two-byte", 0xE9, (const unsigned char[]){195, 169}, 2);
    check_encode("two-byte-max", 0x7FF, (const unsigned char[]){223, 191}, 2);
    check_encode("three-byte-min", 0x800, (const unsigned char[]){224, 160, 128}, 3);
    check_encode("three-byte-max", 0xFFFF, (const unsigned char[]){239, 191, 191}, 3);
    check_encode("four-byte-min", 0x10000, (const unsigned char[]){240, 144, 128, 128}, 4);
    check_encode("max", 0x10FFFF, (const unsigned char[]){244, 143, 191, 191}, 4);
    check_decode("ascii-a", (const unsigned char[]){65}, 1, 65);
    check_decode("e-acute", (const unsigned char[]){195, 169}, 2, 233);
    check_decode("emoji", (const unsigned char[]){240, 159, 153, 130}, 4, 128578);
    check_decode("max", (const unsigned char[]){244, 143, 191, 191}, 4, 1114111);
    check_decode("replacement", (const unsigned char[]){239, 191, 189}, 3, 65533);
    check_decode("empty", NULL, 0, -1);
    check_decode("stray-continuation", (const unsigned char[]){128}, 1, -1);
    check_decode("truncated", (const unsigned char[]){195}, 1, -1);
    check_decode("bad-continuation", (const unsigned char[]){195, 40}, 2, -1);
    check_decode("overlong-two", (const unsigned char[]){192, 129}, 2, -1);
    check_decode("overlong-three", (const unsigned char[]){224, 128, 128}, 3, -1);
    check_decode("surrogate", (const unsigned char[]){237, 160, 128}, 3, -1);
    check_decode("above-max", (const unsigned char[]){244, 144, 128, 128}, 4, -1);
    check_decode("two-sequences", (const unsigned char[]){65, 66}, 2, -1);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
