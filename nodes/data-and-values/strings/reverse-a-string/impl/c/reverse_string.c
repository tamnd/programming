// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-string/.

#include "reverse_string.h"

#include <errno.h>
#include <string.h>

// utf8_len returns the length of the UTF-8 sequence at the start of s[0..n),
// or 0 if the bytes are not valid UTF-8 (RFC 3629). It rejects overlong
// forms, surrogates and values above U+10FFFF.
static size_t utf8_len(const unsigned char *s, size_t n) {
    unsigned char c = s[0];
    size_t len;
    unsigned char lo = 0x80, hi = 0xBF; // the range of the second byte
    if (c < 0x80) {
        return 1;
    } else if (c >= 0xC2 && c <= 0xDF) {
        len = 2;
    } else if (c >= 0xE0 && c <= 0xEF) {
        len = 3;
        if (c == 0xE0) lo = 0xA0; // no overlong form
        if (c == 0xED) hi = 0x9F; // no surrogate
    } else if (c >= 0xF0 && c <= 0xF4) {
        len = 4;
        if (c == 0xF0) lo = 0x90; // no overlong form
        if (c == 0xF4) hi = 0x8F; // no value above U+10FFFF
    } else {
        return 0;
    }
    if (n < len || s[1] < lo || s[1] > hi) return 0;
    for (size_t j = 2; j < len; j++) {
        if ((s[j] & 0xC0) != 0x80) return 0;
    }
    return len;
}

int reverse_code_points(const char *s, size_t n, char *out) {
    const unsigned char *u = (const unsigned char *)s;
    for (size_t i = 0; i < n;) {
        size_t len = utf8_len(u + i, n - i);
        if (len == 0) {
            errno = EILSEQ;
            return -1;
        }
        // The sequence at s[i] goes to the mirror position, with its bytes
        // in the same order.
        memcpy(out + (n - i - len), s + i, len);
        i += len;
    }
    return 0;
}
