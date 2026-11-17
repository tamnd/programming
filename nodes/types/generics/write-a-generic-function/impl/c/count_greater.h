// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-generic-function/.

#ifndef COUNT_GREATER_H
#define COUNT_GREATER_H

#include <stddef.h>

// Returns how many of the count items, each size bytes, compare greater than limit.
// cmp returns a negative number, zero or a positive number, as for qsort.
size_t count_greater(const void *items, size_t count, size_t size, const void *limit,
                     int (*cmp)(const void *, const void *));

// Compares two ints without subtraction, so INT_MIN does not overflow.
int compare_ints(const void *a, const void *b);

// The entry for int: count_greater with compare_ints.
size_t count_greater_int(const int *items, size_t count, int limit);

#endif
