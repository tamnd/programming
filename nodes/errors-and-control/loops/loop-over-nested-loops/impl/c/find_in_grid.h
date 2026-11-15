// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-nested-loops/.

#ifndef FIND_IN_GRID_H
#define FIND_IN_GRID_H

#include <stddef.h>

typedef struct {
    int row;
    int col;
} cell;

// find_in_grid searches n rows. Row r has lens[r] numbers at rows[r]. It returns
// the row and the column of the first cell that equals target, or {-1, -1}.
cell find_in_grid(const int *const *rows, const size_t *lens, size_t n, int target);

#endif
