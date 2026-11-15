// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-counted-loop/.

#include "count_to.h"

#include <stdlib.h>

int *count_to(int n) {
    // malloc(0) may return NULL, so ask for at least one number.
    int *out = malloc((size_t)(n > 0 ? n : 1) * sizeof *out);
    if (out == NULL) {
        return NULL;
    }
    for (int i = 1; i <= n; i++) {
        out[i - 1] = i;
    }
    return out;
}
