// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-combinations-of-a-list/.

#include "combinations.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// C has no empty array, so an empty list has a 0 in it. N, M and C give the
// real lengths: N items, M items in all the choices, C choices.
#define CHECK(name, n, given, k, m, c, flat)                                   \
    do {                                                                       \
        const int32_t g[] = given;                                             \
        const int32_t want[] = flat;                                           \
        int32_t out[sizeof want / sizeof want[0]];                             \
        size_t count = 99;                                                     \
        int ok = combinations(g, n, k, out, m, &count) == 0 && count == c;     \
        ok = ok && memcmp(out, want, (size_t)(m) * sizeof out[0]) == 0;        \
        if (!ok) {                                                             \
            fprintf(stderr, "case %s failed\n", name);                         \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("two-of-four", 4, L(1, 2, 3, 4), 2, 12, 6, L(1, 2, 1, 3, 1, 4, 2, 3, 2, 4, 3, 4));
    CHECK("three-of-four", 4, L(1, 2, 3, 4), 3, 12, 4, L(1, 2, 3, 1, 2, 4, 1, 3, 4, 2, 3, 4));
    CHECK("zero-k", 2, L(1, 2), 0, 0, 1, L(0));
    CHECK("empty", 0, L(0), 0, 0, 1, L(0));
    CHECK("k-too-large", 2, L(1, 2), 3, 0, 0, L(0));
    CHECK("all", 3, L(1, 2, 3), 3, 3, 1, L(1, 2, 3));
    CHECK("duplicates", 3, L(1, 1, 2), 2, 6, 3, L(1, 1, 1, 2, 1, 2));
    CHECK("unsorted", 3, L(3, 1, 2), 2, 6, 3, L(3, 1, 3, 2, 1, 2));
    CHECK("extremes", 2, L(INT32_MIN, INT32_MAX), 1, 2, 2, L(INT32_MIN, INT32_MAX));

    // out has room for 5 items, and the 3 choices of 2 need 6.
    const int32_t three[] = {1, 2, 3};
    int32_t small[5] = {0, 0, 0, 0, 0};
    size_t count = 0;
    if (combinations(three, 3, 2, small, 5, &count) != -1 || small[0] != 0) {
        fprintf(stderr, "case small failed\n");
        failures++;
    }

    // 33 items are more than COMBINATIONS_MAX.
    const int32_t many[33] = {0};
    if (combinations(many, 33, 1, small, 5, &count) != -1) {
        fprintf(stderr, "case max failed\n");
        failures++;
    }

    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
