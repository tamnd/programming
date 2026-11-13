// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-a-list/.

#include "keep_odd.h"

size_t keep_odd(const int *items, size_t n, int *out) {
    size_t k = 0;
    for (size_t i = 0; i < n; i++) {
        // Use != 0, not == 1: the remainder of a negative item can be negative.
        if (items[i] % 2 != 0) {
            out[k++] = items[i];
        }
    }
    return k;
}
