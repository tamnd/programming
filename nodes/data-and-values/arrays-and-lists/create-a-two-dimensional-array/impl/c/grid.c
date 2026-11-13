// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-two-dimensional-array/.

#include "grid.h"

#include <stdint.h>
#include <stdlib.h>

int *grid(size_t rows, size_t cols, int value) {
    if (cols != 0 && rows > SIZE_MAX / sizeof(int) / cols) {
        return NULL;
    }
    size_t n = rows * cols;
    // malloc(0) may return NULL, so ask for at least one item.
    int *g = malloc((n > 0 ? n : 1) * sizeof *g);
    if (g == NULL) {
        return NULL;
    }
    for (size_t k = 0; k < n; k++) {
        g[k] = value;
    }
    return g;
}
