// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-nested-loops/.

#include "find_in_grid.h"

cell find_in_grid(const int *const *rows, const size_t *lens, size_t n, int target) {
    for (size_t r = 0; r < n; r++) {
        for (size_t c = 0; c < lens[r]; c++) {
            if (rows[r][c] == target) {
                // return leaves both loops. break would leave only the inner one.
                return (cell){(int)r, (int)c};
            }
        }
    }
    return (cell){-1, -1};
}
