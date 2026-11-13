// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-permutations-of-a-list/.

#include "permutations.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// C has no empty array, so an empty list has a 0 in it. N, M and C give the
// real lengths: N items, M items in all the orders, C orders.
#define CHECK(name, n, given, m, c, flat)                                      \
    do {                                                                       \
        const int32_t g[] = given;                                             \
        const int32_t want[] = flat;                                           \
        int32_t out[sizeof want / sizeof want[0]];                             \
        size_t count = 99;                                                     \
        int ok = permutations(g, n, out, m, &count) == 0 && count == c;        \
        ok = ok && memcmp(out, want, (size_t)(m) * sizeof out[0]) == 0;        \
        if (!ok) {                                                             \
            fprintf(stderr, "case %s failed\n", name);                         \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("three", 3, L(1, 2, 3), 18, 6, L(1, 2, 3, 1, 3, 2, 2, 1, 3, 2, 3, 1, 3, 1, 2, 3, 2, 1));
    CHECK("two", 2, L(5, 6), 4, 2, L(5, 6, 6, 5));
    CHECK("empty", 0, L(0), 0, 1, L(0));
    CHECK("single", 1, L(7), 1, 1, L(7));
    CHECK("duplicates", 3, L(1, 1, 2), 18, 6, L(1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 2, 1, 1));
    CHECK("unsorted", 3, L(3, 1, 2), 18, 6, L(3, 1, 2, 3, 2, 1, 1, 3, 2, 1, 2, 3, 2, 3, 1, 2, 1, 3));
    CHECK("extremes", 2, L(INT32_MIN, INT32_MAX), 4, 2, L(INT32_MIN, INT32_MAX, INT32_MAX, INT32_MIN));

    // out has room for 3 items, and the 2 orders need 4.
    const int32_t two[] = {1, 2};
    int32_t small[3] = {0, 0, 0};
    size_t count = 0;
    if (permutations(two, 2, small, 3, &count) != -1 || small[0] != 0) {
        fprintf(stderr, "case small failed\n");
        failures++;
    }

    // 13 items are more than PERMUTATIONS_MAX.
    const int32_t many[13] = {0};
    if (permutations(many, 13, small, 3, &count) != -1) {
        fprintf(stderr, "case max failed\n");
        failures++;
    }

    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
