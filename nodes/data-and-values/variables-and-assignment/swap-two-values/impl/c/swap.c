// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/swap-two-values/.

#include "swap.h"

void swap_items(long long *xs, size_t i, size_t j) {
    // A temporary variable keeps the first value while the second value
    // takes its place.
    long long tmp = xs[i];
    xs[i] = xs[j];
    xs[j] = tmp;
}
