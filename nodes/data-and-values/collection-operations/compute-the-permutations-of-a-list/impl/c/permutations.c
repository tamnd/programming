// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-permutations-of-a-list/.

#include "permutations.h"

#include <stdbool.h>

// next_order changes idx to the next order of the indices in lexicographic
// order. It returns false when idx is the last order.
static bool next_order(size_t *idx, size_t n) {
    if (n < 2) {
        return false;
    }
    // Find the last j with idx[j] < idx[j + 1].
    size_t j = n - 1;
    while (j > 0 && idx[j - 1] > idx[j]) {
        j--;
    }
    if (j == 0) {
        return false;
    }
    j--;
    // Find the last l with idx[j] < idx[l], and swap them.
    size_t l = n - 1;
    while (idx[l] < idx[j]) {
        l--;
    }
    size_t t = idx[j];
    idx[j] = idx[l];
    idx[l] = t;
    // Reverse the tail after j.
    for (size_t a = j + 1, b = n - 1; a < b; a++, b--) {
        t = idx[a];
        idx[a] = idx[b];
        idx[b] = t;
    }
    return true;
}

int permutations(const int32_t *items, size_t n, int32_t *out, size_t cap, size_t *count) {
    if (n > PERMUTATIONS_MAX) {
        return -1;
    }
    size_t total = 1;
    for (size_t k = 2; k <= n; k++) {
        total *= k;
    }
    if (n > 0 && total > cap / n) {
        return -1;
    }
    // The indices are all different, so their orders are the orders of the
    // positions, also when items has equal values.
    size_t idx[PERMUTATIONS_MAX];
    for (size_t k = 0; k < n; k++) {
        idx[k] = k;
    }
    size_t r = 0;
    do {
        for (size_t k = 0; k < n; k++) {
            out[r * n + k] = items[idx[k]];
        }
        r++;
    } while (next_order(idx, n));
    *count = total;
    return 0;
}
