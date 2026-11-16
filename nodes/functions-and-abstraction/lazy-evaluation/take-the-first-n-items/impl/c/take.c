// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/take-the-first-n-items/.

#include "take.h"

#include <string.h>

size_t take(const int *items, size_t len, size_t n, int *out) {
    size_t k = n < len ? n : len;
    if (k > 0) {
        memcpy(out, items, k * sizeof *items);
    }
    return k;
}

int naturals_next(struct naturals *seq) {
    return seq->next++;
}

size_t take_naturals(int start, size_t n, int *out) {
    struct naturals seq = {start};
    // The loop asks the sequence for n values and no more.
    for (size_t i = 0; i < n; i++) {
        out[i] = naturals_next(&seq);
    }
    return n;
}
