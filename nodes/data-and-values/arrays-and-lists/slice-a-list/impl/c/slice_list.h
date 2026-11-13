// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/slice-a-list/.

#ifndef SLICE_LIST_H
#define SLICE_LIST_H

#include <stddef.h>

// slice_list returns a new array of the stop - start items items[start] up to,
// but not including, items[stop]. The caller must free it. It returns NULL
// when start > stop, when stop > n, or when malloc fails.
int *slice_list(const int *items, size_t n, size_t start, size_t stop);

#endif
