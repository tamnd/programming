// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-two-lists/.

#include "concat.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int *concat(const int *a, size_t n, const int *b, size_t m) {
    if (n > SIZE_MAX / sizeof(int) - m) {
        return NULL;
    }
    size_t k = n + m;
    // malloc(0) may return NULL, so ask for at least one item.
    int *c = malloc((k > 0 ? k : 1) * sizeof *c);
    if (c == NULL) {
        return NULL;
    }
    // memcpy counts bytes, so multiply the counts by the item size.
    if (n > 0) {
        memcpy(c, a, n * sizeof *a);
    }
    if (m > 0) {
        memcpy(c + n, b, m * sizeof *b);
    }
    return c;
}
