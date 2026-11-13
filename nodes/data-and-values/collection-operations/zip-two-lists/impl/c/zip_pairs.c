// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/zip-two-lists/.

#include "zip_pairs.h"

size_t zip_pairs(const int *a, size_t na, const int *b, size_t nb, struct int_pair *out) {
    // Compare the lengths. Do not subtract them: size_t is unsigned.
    size_t n = na < nb ? na : nb;
    for (size_t i = 0; i < n; i++) {
        out[i].first = a[i];
        out[i].second = b[i];
    }
    return n;
}
