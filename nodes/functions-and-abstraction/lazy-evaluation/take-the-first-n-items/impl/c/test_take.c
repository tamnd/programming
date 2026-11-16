// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/take-the-first-n-items/.

#include "take.h"

#include <stdio.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// An empty list has a 0 in it, because C has no empty array. The counts give
// the real lengths.
#define CHECK(name, len, given, n, m, expected)                                \
    do {                                                                       \
        const int g[] = given;                                                 \
        const int w[] = expected;                                              \
        int out[sizeof g / sizeof g[0]];                                       \
        size_t k = take(g, len, n, out);                                       \
        int ok = k == (size_t)(m);                                             \
        for (size_t i = 0; ok && i < k; i++) {                                 \
            ok = out[i] == w[i];                                               \
        }                                                                      \
        if (!ok) {                                                             \
            fprintf(stderr, "case %s failed\n", name);                         \
            failures++;                                                        \
        }                                                                      \
    } while (0)

#define CHECK_NATURALS(name, start, n, expected)                               \
    do {                                                                       \
        const int w[] = expected;                                              \
        int out[8];                                                            \
        size_t k = take_naturals(start, n, out);                               \
        int ok = k == (size_t)(n);                                             \
        for (size_t i = 0; ok && i < k; i++) {                                 \
            ok = out[i] == w[i];                                               \
        }                                                                      \
        if (!ok) {                                                             \
            fprintf(stderr, "case %s failed\n", name);                         \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("first-three", 5, L(1, 2, 3, 4, 5), 3, 3, L(1, 2, 3));
    CHECK("first-two", 4, L(10, 20, 30, 40), 2, 2, L(10, 20));
    CHECK("zero", 2, L(1, 2), 0, 0, L(0));
    CHECK("empty", 0, L(0), 3, 0, L(0));
    CHECK("more-than-length", 2, L(1, 2), 5, 2, L(1, 2));
    CHECK("equal-length", 3, L(4, 5, 6), 3, 3, L(4, 5, 6));
    CHECK("one", 2, L(9, 8), 1, 1, L(9));
    CHECK("negative", 3, L(-1, -2, -3), 2, 2, L(-1, -2));
    CHECK("extremes", 3, L((-2147483647 - 1), 2147483647, 0), 2, 2, L((-2147483647 - 1), 2147483647));
    CHECK_NATURALS("from-zero", 0, 5, L(0, 1, 2, 3, 4));
    CHECK_NATURALS("from-ten", 10, 3, L(10, 11, 12));
    CHECK_NATURALS("none", 7, 0, L(0));
    CHECK_NATURALS("one", 0, 1, L(0));
    CHECK_NATURALS("large-start", 999998, 2, L(999998, 999999));
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
