// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/partition-a-list/.

#ifndef PARTITION_ODD_H
#define PARTITION_ODD_H

#include <stddef.h>

// partition_odd writes the odd items of items[0..n) to odd and the even items
// to even, each in input order. odd and even must each have room for n items.
// It sets *n_odd and *n_even to the counts.
void partition_odd(const int *items, size_t n, int *odd, size_t *n_odd, int *even, size_t *n_even);

#endif
