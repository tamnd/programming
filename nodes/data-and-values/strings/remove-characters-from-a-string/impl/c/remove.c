// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-characters-from-a-string/.

#include "remove.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// seq_len returns the length of the UTF-8 sequence that starts with the byte b.
static size_t seq_len(unsigned char b) {
    if (b >= 0xf0) {
        return 4;
    }
    if (b >= 0xe0) {
        return 3;
    }
    if (b >= 0xc0) {
        return 2;
    }
    return 1;
}

// in_set reports whether the n bytes at p are one of the code points of chars.
// strchr would compare single bytes, and then it would remove a part of a
// multibyte sequence, so the loop compares whole sequences.
static bool in_set(const char *p, size_t n, const char *chars) {
    while (*chars != '\0') {
        size_t m = seq_len((unsigned char)*chars);
        if (m == n && memcmp(p, chars, n) == 0) {
            return true;
        }
        chars += m;
    }
    return false;
}

char *remove_chars(const char *s, const char *chars) {
    char *out = malloc(strlen(s) + 1);
    if (out == NULL) {
        return NULL;
    }
    size_t j = 0;
    while (*s != '\0') {
        size_t n = seq_len((unsigned char)*s);
        if (!in_set(s, n, chars)) {
            memcpy(out + j, s, n);
            j += n;
        }
        s += n;
    }
    out[j] = '\0';
    return out;
}
