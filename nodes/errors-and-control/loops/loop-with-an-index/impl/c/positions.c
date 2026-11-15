// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-an-index/.

#include "positions.h"

#include <string.h>

size_t positions(const char *const *items, size_t n, const char *target, size_t *out) {
    size_t k = 0;
    for (size_t i = 0; i < n; i++) {
        // strcmp returns 0 when the strings are equal. == would compare the pointers.
        if (strcmp(items[i], target) == 0) {
            out[k++] = i;
        }
    }
    return k;
}
