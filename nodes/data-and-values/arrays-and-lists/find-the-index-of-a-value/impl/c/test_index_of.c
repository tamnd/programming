// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-index-of-a-value/.

#include "index_of.h"

#include <limits.h>
#include <stddef.h>
#include <stdio.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// C has no empty array, so an empty list is the array {0} with a count of 0.
#define CHECK(name, n, items, value, want)              \
    do {                                                \
        const int x[] = items;                          \
        if (index_of(x, n, value) != (want)) {          \
            fprintf(stderr, "case %s failed\n", name); \
            failures++;                                 \
        }                                               \
    } while (0)

int main(void) {
    CHECK("middle", 3, L(4, 7, 1), 7, 1);
    CHECK("absent", 3, L(4, 7, 1), 9, -1);
    CHECK("empty", 0, L(0), 0, -1);
    CHECK("single", 1, L(5), 5, 0);
    CHECK("single-absent", 1, L(5), 0, -1);
    CHECK("first", 3, L(7, 8, 9), 7, 0);
    CHECK("last", 3, L(7, 8, 9), 9, 2);
    CHECK("duplicates", 4, L(3, 5, 3, 5), 5, 1);
    CHECK("all-same", 3, L(2, 2, 2), 2, 0);
    CHECK("negative", 3, L(0, -1, -2), -2, 2);
    CHECK("limit", 2, L(INT_MAX, INT_MIN), INT_MIN, 1);
    CHECK("limit-absent", 1, L(INT_MAX), INT_MIN, -1);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
