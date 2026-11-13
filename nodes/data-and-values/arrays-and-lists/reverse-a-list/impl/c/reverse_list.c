// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-list/.

#include "reverse_list.h"

#include <stdlib.h>

int *reverse_list(const int *items, size_t n) {
    // malloc(0) may return NULL, so ask for at least one item.
    int *out = malloc((n > 0 ? n : 1) * sizeof *out);
    if (out == NULL) {
        return NULL;
    }
    // i counts up, so the loop does not need an unsigned index below 0.
    for (size_t i = 0; i < n; i++) {
        out[i] = items[n - 1 - i];
    }
    return out;
}
