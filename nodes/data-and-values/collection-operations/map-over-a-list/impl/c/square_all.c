// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/map-over-a-list/.

#include "square_all.h"

void square_all(const int *items, size_t n, int *out) {
    for (size_t i = 0; i < n; i++) {
        out[i] = items[i] * items[i];
    }
}
