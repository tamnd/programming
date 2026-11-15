// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-a-step/.

#include "count_by.h"

#include <stdlib.h>

int *count_by(int start, int stop, int step, size_t *count) {
    // long long values: with an int, i + step can overflow, which is undefined.
    long long span = (long long)stop - start;
    size_t n = 0;
    if ((step > 0 && span >= 0) || (step < 0 && span <= 0)) {
        n = (size_t)(span / step) + 1;
    }
    // malloc(0) may return NULL, so ask for at least one value.
    int *out = malloc((n > 0 ? n : 1) * sizeof *out);
    if (out == NULL) {
        return NULL;
    }
    size_t k = 0;
    for (long long i = start; step > 0 ? i <= stop : i >= stop; i += step) {
        out[k++] = (int)i;
    }
    *count = k;
    return out;
}
