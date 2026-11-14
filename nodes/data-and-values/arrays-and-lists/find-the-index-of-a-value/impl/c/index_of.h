// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-index-of-a-value/.

#ifndef INDEX_OF_H
#define INDEX_OF_H

#include <stddef.h>

// index_of returns the first index of value in the n items, or -1 if it is
// not there. The result is a ptrdiff_t, a signed type, so -1 stays -1.
ptrdiff_t index_of(const int *items, size_t n, int value);

#endif
