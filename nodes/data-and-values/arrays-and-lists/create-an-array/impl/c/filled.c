// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-an-array/.

#include "filled.h"

#include <stdlib.h>

int *filled(size_t n, int value) {
    // malloc(0) may return NULL or a pointer, so ask for at least one item.
    // Then NULL always means that the allocation failed.
    int *a = malloc((n > 0 ? n : 1) * sizeof *a);
    if (a == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < n; i++) {
        a[i] = value;
    }
    return a;
}
