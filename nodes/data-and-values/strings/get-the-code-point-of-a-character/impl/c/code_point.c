// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-code-point-of-a-character/.

#include "code_point.h"

// code_point decodes one UTF-8 sequence. The first byte gives the length and
// the high bits of the code point. Each continuation byte, 10xxxxxx, gives 6
// more bits. c[0] alone would be only the first byte.
uint32_t code_point(const char *c) {
    const unsigned char *p = (const unsigned char *)c;
    if (p[0] < 0x80) {
        return p[0];
    }
    if (p[0] < 0xe0) {
        return ((uint32_t)(p[0] & 0x1f) << 6) | (p[1] & 0x3f);
    }
    if (p[0] < 0xf0) {
        return ((uint32_t)(p[0] & 0x0f) << 12) | ((uint32_t)(p[1] & 0x3f) << 6) | (p[2] & 0x3f);
    }
    return ((uint32_t)(p[0] & 0x07) << 18) | ((uint32_t)(p[1] & 0x3f) << 12) | ((uint32_t)(p[2] & 0x3f) << 6) |
           (p[3] & 0x3f);
}
