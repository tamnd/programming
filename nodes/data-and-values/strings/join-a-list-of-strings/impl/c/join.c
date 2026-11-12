// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/join-a-list-of-strings/.

#include "join.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *join(const char *const *items, size_t n, const char *sep) {
    // The first pass adds up the lengths, so the code allocates once.
    size_t seplen = strlen(sep);
    size_t total = 1;
    for (size_t i = 0; i < n; i++) {
        total += strlen(items[i]) + (i > 0 ? seplen : 0);
    }
    char *r = malloc(total);
    if (r == NULL) {
        return NULL;
    }
    char *p = r;
    for (size_t i = 0; i < n; i++) {
        if (i > 0) {
            memcpy(p, sep, seplen);
            p += seplen;
        }
        size_t len = strlen(items[i]);
        memcpy(p, items[i], len);
        p += len;
    }
    *p = '\0';
    return r;
}

char *join_numbers(const int32_t *items, size_t n, const char *sep) {
    // An int32_t has at most 11 characters, "-2147483648", plus the null byte.
    char (*digits)[12] = malloc((n ? n : 1) * sizeof *digits);
    const char **parts = malloc((n ? n : 1) * sizeof *parts);
    char *r = NULL;
    if (digits != NULL && parts != NULL) {
        for (size_t i = 0; i < n; i++) {
            snprintf(digits[i], sizeof digits[i], "%" PRId32, items[i]);
            parts[i] = digits[i];
        }
        r = join(parts, n, sep);
    }
    free(parts);
    free(digits);
    return r;
}
