// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sum-and-multiply-a-list/.

#include "sumprod.h"

// A signed overflow in C is undefined behavior, so the caller must keep
// each partial result in range.
void sum_and_product(const long long *items, size_t n, long long *sum, long long *product) {
    long long s = 0;
    long long p = 1;
    for (size_t i = 0; i < n; i++) {
        s += items[i];
        p *= items[i];
    }
    *sum = s;
    *product = p;
}
