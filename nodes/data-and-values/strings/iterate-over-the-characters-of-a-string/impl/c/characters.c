// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-the-characters-of-a-string/.

#include "characters.h"

#include <string.h>

// seq_len returns the length of the UTF-8 sequence that starts with the byte b.
// An index loop, s[i], would give single bytes, and a code point above U+007F
// has 2 to 4 bytes.
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

char **characters(const char *s, size_t *n) {
    // Each code point has at least one byte, so strlen(s) slots are enough.
    char **out = malloc((strlen(s) + 1) * sizeof *out);
    *n = 0;
    if (out == NULL) {
        return NULL;
    }
    while (*s != '\0') {
        size_t len = seq_len((unsigned char)*s);
        char *c = malloc(len + 1);
        if (c == NULL) {
            free_characters(out, *n);
            *n = 0;
            return NULL;
        }
        memcpy(c, s, len);
        c[len] = '\0';
        out[(*n)++] = c;
        s += len;
    }
    return out;
}

void free_characters(char **chars, size_t n) {
    if (chars == NULL) {
        return;
    }
    for (size_t i = 0; i < n; i++) {
        free(chars[i]);
    }
    free(chars);
}
