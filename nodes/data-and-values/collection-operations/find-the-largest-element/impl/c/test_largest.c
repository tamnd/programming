// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-largest-element/.

#include "largest.h"

#include <stdbool.h>
#include <stdio.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

#define CHECK(name, n, given, want)                                      \
    do {                                                                 \
        const int g[] = given;                                           \
        int got = largest(g, n);                                             \
        if (got != (want)) {                                             \
            fprintf(stderr, "case %s: got %d, want %d\n", name, got, want); \
            failures++;                                                  \
        }                                                                \
    } while (0)

int main(void) {
    CHECK("digits", 5, L(3, 1, 4, 1, 5), 5);
    CHECK("middle", 3, L(10, 20, 15), 20);
    CHECK("single", 1, L(7), 7);
    CHECK("all-negative", 3, L(-3, -1, -2), -1);
    CHECK("first", 3, L(9, 2, 3), 9);
    CHECK("last", 3, L(1, 2, 9), 9);
    CHECK("duplicates", 4, L(5, 9, 9, 1), 9);
    CHECK("all-same", 3, L(4, 4, 4), 4);
    CHECK("zero", 3, L(-5, 0, -1), 0);
    CHECK("extremes", 3, L((-2147483647 - 1), 2147483647, 0), 2147483647);
    CHECK("min-only", 1, L((-2147483647 - 1)), (-2147483647 - 1));
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
