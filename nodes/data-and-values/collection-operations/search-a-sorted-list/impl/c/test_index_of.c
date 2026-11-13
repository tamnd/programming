// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-a-sorted-list/.

#include "index_of.h"

#include <stdio.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// An empty list has a 0 in it, because C has no empty array. The count n
// gives the real length.
#define CHECK(name, n, given, target, want)                                   \
    do {                                                                      \
        const int g[] = given;                                                \
        long got = index_of(g, n, target);                                    \
        if (got != (want)) {                                                  \
            fprintf(stderr, "case %s: got %ld, want %d\n", name, got, want);  \
            failures++;                                                       \
        }                                                                     \
    } while (0)

int main(void) {
    CHECK("middle", 5, L(1, 3, 5, 7, 9), 5, 2);
    CHECK("first", 5, L(1, 3, 5, 7, 9), 1, 0);
    CHECK("last", 5, L(1, 3, 5, 7, 9), 9, 4);
    CHECK("missing", 5, L(1, 3, 5, 7, 9), 4, -1);
    CHECK("below", 3, L(1, 3, 5), 0, -1);
    CHECK("above", 3, L(1, 3, 5), 6, -1);
    CHECK("empty", 0, L(0), 1, -1);
    CHECK("single-hit", 1, L(7), 7, 0);
    CHECK("single-miss", 1, L(7), 8, -1);
    CHECK("duplicates", 5, L(1, 2, 2, 2, 3), 2, 1);
    CHECK("all-same", 4, L(4, 4, 4, 4), 4, 0);
    CHECK("negative", 4, L(-9, -5, -1, 0), -5, 1);
    CHECK("extremes", 3, L((-2147483647 - 1), 0, 2147483647), 2147483647, 2);
    CHECK("min-target", 3, L((-2147483647 - 1), 0, 2147483647), (-2147483647 - 1), 0);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
