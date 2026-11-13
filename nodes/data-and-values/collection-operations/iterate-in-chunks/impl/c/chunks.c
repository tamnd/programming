// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-in-chunks/.

#include "chunks.h"

size_t chunk_count(size_t n, size_t size) {
    // (n + size - 1) / size can wrap around when n is near SIZE_MAX.
    return n / size + (n % size != 0);
}

size_t chunks(const int *items, size_t n, size_t size, struct chunk *out) {
    size_t k = 0;
    for (size_t i = 0; i < n; i += size) {
        out[k].start = items + i;
        out[k].len = n - i < size ? n - i : size;
        k++;
        if (n - i <= size) {
            break;
        }
    }
    return k;
}
