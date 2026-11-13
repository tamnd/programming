// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-in-chunks/.

#include "chunks.h"

#include <stdint.h>
#include <stdio.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// An empty list has a 0 in it, because C has no empty array. The counts give
// the real lengths. LENS has the length of each chunk.
#define CHECK(name, n, given, size, m, lens)                                   \
    do {                                                                       \
        const int g[] = given;                                                 \
        const size_t w[] = lens;                                               \
        struct chunk out[sizeof g / sizeof g[0]];                              \
        const size_t want_n = m;                                               \
        size_t k = chunks(g, n, size, out);                                    \
        int ok = k == want_n && chunk_count(n, size) == want_n;                \
        size_t at = 0;                                                         \
        for (size_t i = 0; ok && i < want_n; i++) {                            \
            ok = out[i].start == g + at && out[i].len == w[i];                 \
            at += w[i];                                                        \
        }                                                                      \
        if (!ok) {                                                             \
            fprintf(stderr, "case %s failed\n", name);                         \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("pairs", 5, L(1, 2, 3, 4, 5), 2, 3, L(2, 2, 1));
    CHECK("triples", 6, L(1, 2, 3, 4, 5, 6), 3, 2, L(3, 3));
    CHECK("empty", 0, L(0), 3, 0, L(0));
    CHECK("size-one", 3, L(7, 8, 9), 1, 3, L(1, 1, 1));
    CHECK("size-larger", 2, L(1, 2), 5, 1, L(2));
    CHECK("size-equal", 3, L(1, 2, 3), 3, 1, L(3));
    CHECK("last-shorter", 4, L(1, 2, 3, 4), 3, 2, L(3, 1));
    CHECK("negative", 3, L(-1, -2, -3), 2, 2, L(2, 1));
    CHECK("extremes", 3, L((-2147483647 - 1), 2147483647, 0), 2, 2, L(2, 1));
    if (chunk_count(SIZE_MAX, 2) != SIZE_MAX / 2 + 1) {
        fprintf(stderr, "chunk_count(SIZE_MAX, 2) failed\n");
        failures++;
    }
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
