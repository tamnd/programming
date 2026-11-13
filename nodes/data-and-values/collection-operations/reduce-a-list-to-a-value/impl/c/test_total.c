// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reduce-a-list-to-a-value/.

#include "total.h"

#include <stdbool.h>
#include <stdio.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

#define CHECK(name, n, given, want)                                      \
    do {                                                                 \
        const int g[] = given;                                           \
        int got = total(g, n);                                             \
        if (got != (want)) {                                             \
            fprintf(stderr, "case %s: got %d, want %d\n", name, got, want); \
            failures++;                                                  \
        }                                                                \
    } while (0)

int main(void) {
    CHECK("small", 4, L(1, 2, 3, 4), 10);
    CHECK("mixed", 3, L(10, -3, 5), 12);
    CHECK("empty", 0, L(0), 0);
    CHECK("single", 1, L(7), 7);
    CHECK("negative", 3, L(-1, -2, -3), -6);
    CHECK("zero-sum", 2, L(5, -5), 0);
    CHECK("max", 1, L(2147483647), 2147483647);
    CHECK("min", 1, L((-2147483647 - 1)), (-2147483647 - 1));
    CHECK("to-the-limit", 2, L(2147483646, 1), 2147483647);
    CHECK("partial-overflow", 3, L(2147483647, 1, -1), 2147483647);
    CHECK("partial-underflow", 3, L((-2147483647 - 1), -1, 1), (-2147483647 - 1));
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
