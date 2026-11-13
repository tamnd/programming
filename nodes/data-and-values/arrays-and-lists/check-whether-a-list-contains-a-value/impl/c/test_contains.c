// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

#include "contains.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// C has no empty array, so an empty list is the array {0} with a count of 0.
#define CHECK(name, n, items, value, want)              \
    do {                                                \
        const int x[] = items;                          \
        if (contains(x, n, value) != (want)) {          \
            fprintf(stderr, "case %s failed\n", name); \
            failures++;                                 \
        }                                               \
    } while (0)

int main(void) {
    CHECK("present", 3, L(1, 2, 3), 2, true);
    CHECK("absent", 3, L(1, 2, 3), 4, false);
    CHECK("empty", 0, L(0), 0, false);
    CHECK("single-yes", 1, L(5), 5, true);
    CHECK("single-no", 1, L(5), 0, false);
    CHECK("first", 3, L(7, 8, 9), 7, true);
    CHECK("last", 3, L(7, 8, 9), 9, true);
    CHECK("duplicates", 3, L(2, 2, 2), 2, true);
    CHECK("negative", 3, L(-1, -2, -3), -2, true);
    CHECK("zero-absent", 2, L(-1, 1), 0, false);
    CHECK("limit", 2, L(INT_MAX, INT_MIN), INT_MIN, true);
    CHECK("limit-absent", 1, L(INT_MAX), INT_MIN, false);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
