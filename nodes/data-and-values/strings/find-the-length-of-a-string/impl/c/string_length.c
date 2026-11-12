// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-length-of-a-string/.

#include "string_length.h"

void lengths(const char *s, size_t n, size_t out[3]) {
    size_t code_points = 0, four_byte = 0;
    for (size_t i = 0; i < n; i++) {
        unsigned char b = (unsigned char)s[i];
        // Each code point has one first byte. A continuation byte is 10xxxxxx.
        if ((b & 0xC0) != 0x80) {
            code_points++;
        }
        // A first byte 11110xxx starts a code point above U+FFFF, and UTF-16
        // needs two code units for it.
        if (b >= 0xF0) {
            four_byte++;
        }
    }
    out[0] = n;
    out[1] = code_points + four_byte;
    out[2] = code_points;
}
