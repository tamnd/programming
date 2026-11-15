// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-several-collections-at-once/.

#include "zip_join.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t zip_join(const char *const *lower, size_t nl, const char *const *upper, size_t nu, const int *digits,
                size_t nd, char **out) {
    // An index past the end of a shorter array is undefined behavior.
    size_t n = nl < nu ? nl : nu;
    n = n < nd ? n : nd;
    for (size_t i = 0; i < n; i++) {
        // 12 bytes hold any int in decimal with its sign, and 1 more holds the 0.
        size_t size = strlen(lower[i]) + strlen(upper[i]) + 13;
        out[i] = malloc(size);
        if (out[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(out[j]);
            }
            return (size_t)-1;
        }
        snprintf(out[i], size, "%s%s%d", lower[i], upper[i], digits[i]);
    }
    return n;
}
