// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/change-the-case-of-a-string/.

#include "case.h"

#include <stdlib.h>
#include <string.h>

// toupper depends on the locale, and its argument must be an unsigned char
// value, so the function compares each byte with the ASCII letters. The
// bytes of a UTF-8 sequence are 0x80 or more, so they do not change.
static char *map(const char *s, char lo, char hi, int delta) {
    size_t n = strlen(s);
    char *out = malloc(n + 1);
    if (out == NULL) {
        return NULL;
    }
    for (size_t i = 0; i <= n; i++) {
        char c = s[i];
        out[i] = (c >= lo && c <= hi) ? (char)(c + delta) : c;
    }
    return out;
}

char *to_upper(const char *s) { return map(s, 'a', 'z', 'A' - 'a'); }

char *to_lower(const char *s) { return map(s, 'A', 'Z', 'a' - 'A'); }
