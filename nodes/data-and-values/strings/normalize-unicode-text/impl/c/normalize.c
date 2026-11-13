// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/normalize-unicode-text/.
#include "normalize.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const char vowels[] = "AEIOUaeiou";
static const uint32_t marks[4] = {0x300, 0x301, 0x302, 0x308};

// composites[i][j] is the composite of vowels[i] in upper case and marks[j].
static const uint32_t composites[5][4] = {
    {0xC0, 0xC1, 0xC2, 0xC4}, {0xC8, 0xC9, 0xCA, 0xCB}, {0xCC, 0xCD, 0xCE, 0xCF},
    {0xD2, 0xD3, 0xD4, 0xD6}, {0xD9, 0xDA, 0xDB, 0xDC},
};

static uint32_t composite(size_t i, size_t j) {
    return i >= 5 ? composites[i - 5][j] + 0x20 : composites[i][j];
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

// encode writes the UTF-8 form of c, which is below U+0800, at out.
static char *encode(char *out, uint32_t c) {
    if (c < 0x80) {
        *out++ = (char)c;
    } else {
        *out++ = (char)(0xC0 | (c >> 6));
        *out++ = (char)(0x80 | (c & 0x3F));
    }
    return out;
}

char *normalize(const char *s, const char *form) {
    int nfd = strcmp(form, "NFD") == 0;
    // NFD makes each 2-byte composite 3 bytes, so the result is at most
    // 1.5 times as long as s.
    char *out = malloc(strlen(s) * 3 / 2 + 1);
    if (out == NULL) {
        return NULL;
    }
    char *end = out;
    uint32_t last = 0;      // the last code point written
    char *last_at = NULL;   // where it starts in out
    const unsigned char *p = (const unsigned char *)s;
    while (*p != 0) {
        uint32_t c = decode(&p);
        int done = 0;
        for (size_t i = 0; i < 10 && !done; i++) {
            for (size_t j = 0; j < 4 && !done; j++) {
                if (nfd && composite(i, j) == c) {
                    end = encode(end, (uint32_t)vowels[i]);
                    end = encode(end, marks[j]);
                    done = 1;
                } else if (!nfd && last_at != NULL && last == (uint32_t)vowels[i] && c == marks[j]) {
                    end = encode(last_at, composite(i, j));
                    last = composite(i, j);
                    done = 1;
                }
            }
        }
        if (!done) {
            last_at = end;
            last = c;
            end = encode(end, c);
        }
    }
    *end = 0;
    return out;
}
