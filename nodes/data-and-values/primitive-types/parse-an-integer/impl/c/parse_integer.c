// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-an-integer/.

#include "parse_integer.h"

#include <errno.h>
#include <stdlib.h>

// digit_value returns the value of one ASCII digit or letter, or -1. It does
// not use isalnum or tolower, because their result depends on the locale.
static int digit_value(char c) {
    static const char lower[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    static const char upper[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < 36; i++) {
        if (c == lower[i] || c == upper[i]) {
            return i;
        }
    }
    return -1;
}

enum parse_status parse_integer(const char *s, int base, int64_t *out) {
    // strtoll skips white space, reads a 0x prefix in base 16 and stops at
    // the first bad character, so check the whole string first.
    const char *p = s;
    if (*p == '+' || *p == '-') {
        p++;
    }
    if (*p == '\0') {
        return PARSE_INVALID;
    }
    for (; *p != '\0'; p++) {
        int d = digit_value(*p);
        if (d < 0 || d >= base) {
            return PARSE_INVALID;
        }
    }
    // strtoll sets errno only on an error, so clear it first.
    errno = 0;
    long long value = strtoll(s, NULL, base);
    if (errno == ERANGE) {
        return PARSE_RANGE;
    }
    *out = value;
    return PARSE_OK;
}
