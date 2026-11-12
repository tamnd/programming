// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/encode-and-decode-utf-8/.

#include "utf8.h"

size_t encode_utf8(uint32_t cp, unsigned char out[4]) {
    if (cp < 0x80) {
        out[0] = (unsigned char)cp;
        return 1;
    }
    if (cp < 0x800) {
        out[0] = (unsigned char)(0xc0 | (cp >> 6));
        out[1] = (unsigned char)(0x80 | (cp & 0x3f));
        return 2;
    }
    if (cp < 0x10000) {
        out[0] = (unsigned char)(0xe0 | (cp >> 12));
        out[1] = (unsigned char)(0x80 | ((cp >> 6) & 0x3f));
        out[2] = (unsigned char)(0x80 | (cp & 0x3f));
        return 3;
    }
    out[0] = (unsigned char)(0xf0 | (cp >> 18));
    out[1] = (unsigned char)(0x80 | ((cp >> 12) & 0x3f));
    out[2] = (unsigned char)(0x80 | ((cp >> 6) & 0x3f));
    out[3] = (unsigned char)(0x80 | (cp & 0x3f));
    return 4;
}

// The first byte gives the length and the high bits. Each continuation byte
// must have the form 10xxxxxx. The result must be at least the smallest code
// point of its length, so an overlong form fails, and it must not be a
// surrogate or above 0x10FFFF.
long decode_utf8(const unsigned char *b, size_t n) {
    if (n == 0) {
        return -1;
    }
    size_t len;
    uint32_t cp;
    uint32_t min;
    if (b[0] < 0x80) {
        len = 1, cp = b[0], min = 0;
    } else if (b[0] >= 0xc0 && b[0] < 0xe0) {
        len = 2, cp = b[0] & 0x1f, min = 0x80;
    } else if (b[0] >= 0xe0 && b[0] < 0xf0) {
        len = 3, cp = b[0] & 0x0f, min = 0x800;
    } else if (b[0] >= 0xf0 && b[0] < 0xf8) {
        len = 4, cp = b[0] & 0x07, min = 0x10000;
    } else {
        return -1;
    }
    if (n != len) {
        return -1;
    }
    for (size_t i = 1; i < len; i++) {
        if ((b[i] & 0xc0) != 0x80) {
            return -1;
        }
        cp = (cp << 6) | (b[i] & 0x3f);
    }
    if (cp < min || cp > 0x10ffff || (cp >= 0xd800 && cp <= 0xdfff)) {
        return -1;
    }
    return (long)cp;
}
