// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/trim-whitespace/.

#include "trim.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// isspace depends on the locale, so the function gives the 6 ASCII
// characters. A byte of a UTF-8 sequence is 0x80 or more, so it is not white
// space.
static bool is_space(char c) { return c == ' ' || (c >= '\t' && c <= '\r'); }

static char *copy(const char *start, size_t n) {
    char *out = malloc(n + 1);
    if (out == NULL) {
        return NULL;
    }
    memcpy(out, start, n);
    out[n] = '\0';
    return out;
}

char *trim(const char *s) {
    while (is_space(*s)) {
        s++;
    }
    size_t n = strlen(s);
    while (n > 0 && is_space(s[n - 1])) {
        n--;
    }
    return copy(s, n);
}

char *trim_start(const char *s) {
    while (is_space(*s)) {
        s++;
    }
    return copy(s, strlen(s));
}
