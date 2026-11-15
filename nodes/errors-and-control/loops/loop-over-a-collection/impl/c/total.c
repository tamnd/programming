// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-a-collection/.

#include "total.h"

long long total(const int *items, size_t n) {
    // A long long sum: an int sum can overflow, which is undefined.
    long long s = 0;
    for (size_t i = 0; i < n; i++) {
        s += items[i];
    }
    return s;
}
