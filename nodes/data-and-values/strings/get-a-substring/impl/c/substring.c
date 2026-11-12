// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-substring/.

#include "substring.h"

#include <stdlib.h>
#include <string.h>

// offset returns the byte index of code point k in s, or strlen(s) for the
// end. A byte of the form 10xxxxxx continues a code point, and every other
// byte starts one.
static size_t offset(const char *s, size_t k) {
    size_t n = 0;
    size_t i = 0;
    for (; s[i] != '\0'; i++) {
        if (((unsigned char)s[i] & 0xC0) != 0x80) {
            if (n == k) {
                return i;
            }
            n++;
        }
    }
    return i;
}

char *substring(const char *s, size_t start, size_t end) {
    size_t from = offset(s, start);
    size_t to = offset(s, end);
    char *r = malloc(to - from + 1);
    if (r == NULL) {
        return NULL;
    }
    memcpy(r, s + from, to - from);
    r[to - from] = '\0';
    return r;
}
