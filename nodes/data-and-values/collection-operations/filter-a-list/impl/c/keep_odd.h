// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-a-list/.

#ifndef KEEP_ODD_H
#define KEEP_ODD_H

#include <stddef.h>

// keep_odd copies the odd items of items[0..n) to out, in their order, and
// returns the number of items it copied. out must have room for n items.
size_t keep_odd(const int *items, size_t n, int *out);

#endif
