// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-list/.

#include "insert_at.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int *insert_at(const int *items, size_t n, size_t index, int value) {
    if (index > n || n >= SIZE_MAX / sizeof(int)) {
        return NULL;
    }
    int *out = malloc((n + 1) * sizeof *out);
    if (out == NULL) {
        return NULL;
    }
    // memcpy counts bytes, so multiply the counts by the item size.
    if (index > 0) {
        memcpy(out, items, index * sizeof *items);
    }
    out[index] = value;
    if (n > index) {
        memcpy(out + index + 1, items + index, (n - index) * sizeof *items);
    }
    return out;
}
