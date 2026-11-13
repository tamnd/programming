// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/zip-two-lists/.

#ifndef ZIP_PAIRS_H
#define ZIP_PAIRS_H

#include <stddef.h>

struct int_pair {
    int first;
    int second;
};

// zip_pairs writes the pairs {a[i], b[i]} to out for each i that is less than
// both na and nb. out must have room for the smaller of na and nb. It returns
// the number of pairs.
size_t zip_pairs(const int *a, size_t na, const int *b, size_t nb, struct int_pair *out);

#endif
