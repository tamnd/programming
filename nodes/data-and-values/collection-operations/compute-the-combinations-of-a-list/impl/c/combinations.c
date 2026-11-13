// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-combinations-of-a-list/.

#include "combinations.h"

int combinations(const int32_t *items, size_t n, size_t k, int32_t *out, size_t cap, size_t *count) {
    if (n > COMBINATIONS_MAX) {
        return -1;
    }
    if (k > n) {
        *count = 0;
        return 0;
    }
    // After step i, total is C(n, i + 1). Multiply first, then divide: the
    // division is exact, and a division first would truncate.
    size_t total = 1;
    for (size_t i = 0; i < k; i++) {
        if (total > SIZE_MAX / (n - i)) {
            return -1;
        }
        total = total * (n - i) / (i + 1);
    }
    if (k > 0 && total > cap / k) {
        return -1;
    }
    // idx holds the chosen positions in increasing order, from 0, 1, ...
    // to n - k, ..., n - 1.
    size_t idx[COMBINATIONS_MAX];
    for (size_t j = 0; j < k; j++) {
        idx[j] = j;
    }
    size_t r = 0;
    for (;;) {
        for (size_t j = 0; j < k; j++) {
            out[r * k + j] = items[idx[j]];
        }
        r++;
        // Find the last position that can move right.
        size_t i = k;
        while (i > 0 && idx[i - 1] == n - k + i - 1) {
            i--;
        }
        if (i == 0) {
            break;
        }
        idx[i - 1]++;
        for (size_t j = i; j < k; j++) {
            idx[j] = idx[j - 1] + 1;
        }
    }
    *count = total;
    return 0;
}
