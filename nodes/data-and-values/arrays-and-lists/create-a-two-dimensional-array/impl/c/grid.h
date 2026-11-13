// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-two-dimensional-array/.

#ifndef GRID_H
#define GRID_H

#include <stddef.h>

// grid returns one block of rows * cols ints, each set to value, in row-major
// order: the item at row i and column j is at index i * cols + j. The caller
// must free it. It returns NULL when the size overflows or malloc fails.
int *grid(size_t rows, size_t cols, int value);

#endif
