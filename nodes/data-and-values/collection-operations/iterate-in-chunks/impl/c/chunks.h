// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-in-chunks/.

#ifndef CHUNKS_H
#define CHUNKS_H

#include <stddef.h>

struct chunk {
    const int *start;
    size_t len;
};

// chunk_count returns the number of chunks of n items with size items each.
// size must be at least 1.
size_t chunk_count(size_t n, size_t size);

// chunks writes the chunks of items[0..n) to out and returns their number. Each
// chunk points into items. out must have room for chunk_count(n, size) chunks.
size_t chunks(const int *items, size_t n, size_t size, struct chunk *out);

#endif
