// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-strings/.

#include "concatenate.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *concat(const char *a, const char *b) {
    size_t na = strlen(a), nb = strlen(b);
    char *r = malloc(na + nb + 1);
    if (r == NULL) {
        return NULL;
    }
    // memcpy with the known lengths copies each byte once. strcat would scan
    // the destination again to find its end.
    memcpy(r, a, na);
    memcpy(r + na, b, nb + 1);
    return r;
}

char *concat_number(const char *s, int32_t n) {
    // A call with no buffer returns the length that the text needs.
    int len = snprintf(NULL, 0, "%s%" PRId32, s, n);
    if (len < 0) {
        return NULL;
    }
    char *r = malloc((size_t)len + 1);
    if (r == NULL) {
        return NULL;
    }
    snprintf(r, (size_t)len + 1, "%s%" PRId32, s, n);
    return r;
}
