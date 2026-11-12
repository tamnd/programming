// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-for-a-substring/.

#include "search.h"

#include <string.h>

// code_points counts the code points in the n bytes at s. Each byte that does
// not have the form 10xxxxxx starts a code point.
static long code_points(const char *s, size_t n) {
    long count = 0;
    for (size_t i = 0; i < n; i++) {
        if (((unsigned char)s[i] & 0xC0) != 0x80) {
            count++;
        }
    }
    return count;
}

long find(const char *s, const char *sub) {
    const char *p = strstr(s, sub);
    return p == NULL ? -1 : code_points(s, (size_t)(p - s));
}

long find_last(const char *s, const char *sub) {
    // C has no reverse strstr, so search again after each match.
    const char *last = NULL;
    for (const char *p = strstr(s, sub); p != NULL; p = strstr(p + 1, sub)) {
        last = p;
        if (*p == '\0') {
            break;
        }
    }
    return last == NULL ? -1 : code_points(s, (size_t)(last - s));
}
