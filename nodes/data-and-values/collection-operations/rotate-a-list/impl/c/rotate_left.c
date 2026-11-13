// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rotate-a-list/.

#include "rotate_left.h"

void rotate_left(const int *items, size_t n, size_t k, int *out) {
    if (n == 0) {
        return;
    }
    size_t s = k % n;
    for (size_t i = 0; i < n; i++) {
        out[i] = items[(i + s) % n];
    }
}
