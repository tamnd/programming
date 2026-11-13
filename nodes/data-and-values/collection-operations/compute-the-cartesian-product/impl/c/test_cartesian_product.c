// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-cartesian-product/.

#include "cartesian_product.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// C has no empty array, so an empty list of lists has {NULL, 0} in it, and
// an empty result has a 0 in it. N, M and C give the real lengths: N lists,
// M items in all the tuples, C tuples.
#define CHECK(name, n, given, m, c, flat)                                      \
    do {                                                                       \
        const struct int_list g[] = given;                                     \
        const int32_t want[] = flat;                                           \
        int32_t out[sizeof want / sizeof want[0]];                             \
        size_t count = 99;                                                     \
        int ok = cartesian_product(g, n, out, m, &count) == 0 && count == c;   \
        ok = ok && memcmp(out, want, (size_t)(m) * sizeof out[0]) == 0;        \
        if (!ok) {                                                             \
            fprintf(stderr, "case %s failed\n", name);                         \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("two", 2, L({(const int32_t[]){1, 2}, 2}, {(const int32_t[]){3, 4}, 2}), 8, 4, L(1, 3, 1, 4, 2, 3, 2, 4));
    CHECK("three", 3, L({(const int32_t[]){1, 2}, 2}, {(const int32_t[]){3}, 1}, {(const int32_t[]){4, 5}, 2}), 12, 4, L(1, 3, 4, 1, 3, 5, 2, 3, 4, 2, 3, 5));
    CHECK("no-lists", 0, L({NULL, 0}), 0, 1, L(0));
    CHECK("one-empty", 2, L({(const int32_t[]){1, 2}, 2}, {NULL, 0}), 0, 0, L(0));
    CHECK("single", 1, L({(const int32_t[]){7, 8, 9}, 3}), 3, 3, L(7, 8, 9));
    CHECK("duplicates", 2, L({(const int32_t[]){1, 1}, 2}, {(const int32_t[]){2}, 1}), 4, 2, L(1, 2, 1, 2));
    CHECK("negative", 2, L({(const int32_t[]){-1}, 1}, {(const int32_t[]){-2, -3}, 2}), 4, 2, L(-1, -2, -1, -3));
    CHECK("extremes", 2, L({(const int32_t[]){INT32_MIN, INT32_MAX}, 2}, {(const int32_t[]){0}, 1}), 4, 2, L(INT32_MIN, 0, INT32_MAX, 0));

    // out has room for 3 items, and the 2 tuples need 4.
    const struct int_list two[] = {{(const int32_t[]){1, 2}, 2}, {(const int32_t[]){3}, 1}};
    int32_t small[3] = {0, 0, 0};
    size_t count = 0;
    if (cartesian_product(two, 2, small, 3, &count) != -1 || small[0] != 0) {
        fprintf(stderr, "case small failed\n");
        failures++;
    }

    // The product of the lengths wraps around, so the function must give -1.
    const int32_t one[] = {1};
    const struct int_list huge[] = {{one, SIZE_MAX}, {one, 2}};
    if (cartesian_product(huge, 2, small, 3, &count) != -1) {
        fprintf(stderr, "case wrap failed\n");
        failures++;
    }

    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
