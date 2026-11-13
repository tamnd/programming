// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-graphemes-of-a-string/.
#include "count_graphemes.h"

#include <stdint.h>

enum kind { OTHER, CR, LF, CONTROL, EXTEND, ZWJ, RI, PICT };

// kind returns the Grapheme_Cluster_Break kind of c, or PICT for
// Extended_Pictographic.
static enum kind kind(uint32_t c) {
    if (c == 0x0D) {
        return CR;
    }
    if (c == 0x0A) {
        return LF;
    }
    if (c < 0x20 || (c >= 0x7F && c <= 0x9F) || c == 0xAD) {
        return CONTROL;
    }
    if ((c >= 0x300 && c <= 0x36F) || (c >= 0xFE00 && c <= 0xFE0F) || (c >= 0x1F3FB && c <= 0x1F3FF)) {
        return EXTEND;
    }
    if (c == 0x200D) {
        return ZWJ;
    }
    if (c >= 0x1F1E6 && c <= 0x1F1FF) {
        return RI;
    }
    if (c == 0xA9 || c == 0xAE || c == 0x2764 || (c >= 0x1F400 && c <= 0x1F4FD) || (c >= 0x1F600 && c <= 0x1F64F)) {
        return PICT;
    }
    return OTHER;
}

// decode reads the UTF-8 sequence at *p, and moves *p past it.
static uint32_t decode(const unsigned char **p) {
    const unsigned char *b = *p;
    size_t n = b[0] < 0x80 ? 1 : b[0] < 0xE0 ? 2 : b[0] < 0xF0 ? 3 : 4;
    uint32_t c = n == 1 ? b[0] : (uint32_t)(b[0] & (0x7F >> n));
    for (size_t i = 1; i < n; i++) {
        c = (c << 6) | (uint32_t)(b[i] & 0x3F);
    }
    *p = b + n;
    return c;
}

size_t count_graphemes(const char *s) {
    const unsigned char *p = (const unsigned char *)s;
    size_t n = 0;
    size_t ri_run = 0;
    int have_prev = 0;
    int pict = 0;
    int zwj_after_pict = 0;
    enum kind prev = OTHER;
    while (*p != 0) {
        enum kind k = kind(decode(&p));
        int join = 0;
        if (!have_prev) {
            join = 0;
        } else if (prev == CR && k == LF) {
            join = 1; // GB3
        } else if (prev == CR || prev == LF || prev == CONTROL || k == CR || k == LF || k == CONTROL) {
            join = 0; // GB4, GB5
        } else if (k == EXTEND || k == ZWJ) {
            join = 1; // GB9
        } else if (k == PICT && zwj_after_pict) {
            join = 1; // GB11
        } else if (k == RI && ri_run % 2 == 1) {
            join = 1; // GB12, GB13
        }
        if (!join) {
            n++;
        }
        zwj_after_pict = k == ZWJ && pict;
        pict = k == PICT || (k == EXTEND && pict);
        ri_run = k == RI ? ri_run + 1 : 0;
        prev = k;
        have_prev = 1;
    }
    return n;
}
