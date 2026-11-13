// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reduce-a-list-to-a-value/.

#include "total.h"

int total(const int *items, size_t n) {
    // A long long holds each partial sum, so no int addition overflows.
    long long acc = 0;
    for (size_t i = 0; i < n; i++) {
        acc += items[i];
    }
    return (int)acc;
}
