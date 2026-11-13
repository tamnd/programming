// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-largest-element/.

#include "largest.h"

int largest(const int *items, size_t n) {
    // Start from the first item, not from 0, so a list of negative items works.
    int best = items[0];
    for (size_t i = 1; i < n; i++) {
        if (items[i] > best) {
            best = items[i];
        }
    }
    return best;
}
