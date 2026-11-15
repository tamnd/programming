// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-an-index/.

#ifndef POSITIONS_H
#define POSITIONS_H

#include <stddef.h>

// positions writes the index of each of the n items that equals target to out,
// which must have room for n values, and returns the number of indexes.
size_t positions(const char *const *items, size_t n, const char *target, size_t *out);

#endif
