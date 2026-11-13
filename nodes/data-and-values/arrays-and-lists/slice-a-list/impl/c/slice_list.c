// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/slice-a-list/.

#include "slice_list.h"

#include <stdlib.h>
#include <string.h>

int *slice_list(const int *items, size_t n, size_t start, size_t stop) {
    // Check start <= stop first: stop - start wraps around when start > stop.
    if (start > stop || stop > n) {
        return NULL;
    }
    size_t k = stop - start;
    // malloc(0) may return NULL, so ask for at least one item.
    int *out = malloc((k > 0 ? k : 1) * sizeof *out);
    if (out == NULL) {
        return NULL;
    }
    if (k > 0) {
        memcpy(out, items + start, k * sizeof *items);
    }
    return out;
}
