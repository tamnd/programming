// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-word-count/.

#define _POSIX_C_SOURCE 200809L  // for getline

#include "word_count.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// The 25 code points that have the Unicode property White_Space.
static bool is_white_space(uint32_t c) {
    return (c >= 0x09 && c <= 0x0d) || c == 0x20 || c == 0x85 || c == 0xa0 || c == 0x1680 ||
           (c >= 0x2000 && c <= 0x200a) || c == 0x2028 || c == 0x2029 || c == 0x202f ||
           c == 0x205f || c == 0x3000;
}

// Decodes one UTF-8 sequence at s, with n > 0 bytes left. It returns the
// length of the sequence and stores the code point in *cp, or returns 0 if
// the bytes are not valid UTF-8 (RFC 3629: no overlong forms, no surrogates,
// nothing above U+10FFFF).
static size_t decode(const unsigned char *s, size_t n, uint32_t *cp) {
    if (s[0] < 0x80) {
        *cp = s[0];
        return 1;
    }
    size_t len;
    uint32_t min;
    if (s[0] >= 0xc2 && s[0] <= 0xdf) {
        len = 2, min = 0x80, *cp = s[0] & 0x1fu;
    } else if (s[0] >= 0xe0 && s[0] <= 0xef) {
        len = 3, min = 0x800, *cp = s[0] & 0x0fu;
    } else if (s[0] >= 0xf0 && s[0] <= 0xf4) {
        len = 4, min = 0x10000, *cp = s[0] & 0x07u;
    } else {
        return 0;
    }
    if (n < len) {
        return 0;
    }
    for (size_t i = 1; i < len; i++) {
        if ((s[i] & 0xc0u) != 0x80) {
            return 0;
        }
        *cp = (*cp << 6) | (s[i] & 0x3fu);
    }
    if (*cp < min || *cp > 0x10ffff || (*cp >= 0xd800 && *cp <= 0xdfff)) {
        return 0;
    }
    return len;
}

int word_count(const char *path, struct word_count *out) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return -1;
    }
    struct word_count c = {0, 0, 0};
    char *line = NULL;
    size_t cap = 0;
    ssize_t n;
    // getline reads up to and with the next LF. An LF byte is never part of a
    // longer UTF-8 sequence, so each line decodes on its own.
    while ((n = getline(&line, &cap, f)) != -1) {
        const unsigned char *s = (const unsigned char *)line;
        size_t left = (size_t)n;
        bool in_word = false;
        c.bytes += left;
        while (left > 0) {
            uint32_t cp;
            size_t len = decode(s, left, &cp);
            if (len == 0) {
                free(line);
                fclose(f);
                errno = EILSEQ;
                return -1;
            }
            if (is_white_space(cp)) {
                in_word = false;
                c.lines += cp == '\n';
            } else if (!in_word) {
                in_word = true;
                c.words++;
            }
            s += len;
            left -= len;
        }
    }
    // getline returns -1 both at the end of the file and on an error.
    int err = ferror(f) ? errno : 0;
    free(line);
    fclose(f);
    if (err != 0) {
        errno = err;
        return -1;
    }
    *out = c;
    return 0;
}
