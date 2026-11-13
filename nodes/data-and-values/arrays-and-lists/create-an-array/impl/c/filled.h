// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-an-array/.

#ifndef FILLED_H
#define FILLED_H

#include <stddef.h>

// filled returns a new array of n ints, and each item is value. The caller
// must free the array. It returns NULL when the allocation fails.
int *filled(size_t n, int value);

#endif
