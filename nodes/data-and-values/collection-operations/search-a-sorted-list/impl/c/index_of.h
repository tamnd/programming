// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-a-sorted-list/.

#ifndef INDEX_OF_H
#define INDEX_OF_H

#include <stddef.h>

// index_of returns the first index of target in the sorted array
// items[0..n), or -1.
long index_of(const int *items, size_t n, int target);

#endif
