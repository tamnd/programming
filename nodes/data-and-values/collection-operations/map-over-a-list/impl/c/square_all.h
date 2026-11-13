// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/map-over-a-list/.

#ifndef SQUARE_ALL_H
#define SQUARE_ALL_H

#include <stddef.h>

// square_all writes the square of items[i] to out[i] for each i in [0, n).
// out must have room for n items. Each item x must have |x| <= 46340, so
// that x * x fits in a 32-bit int.
void square_all(const int *items, size_t n, int *out);

#endif
