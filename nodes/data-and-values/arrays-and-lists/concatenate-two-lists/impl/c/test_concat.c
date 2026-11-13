// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-two-lists/.

#include "concat.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

static void check(const char *name, const int *a, size_t n, const int *b, size_t m, const int *want) {
    int *c = concat(a, n, b, m);
    if (c == NULL || (n + m > 0 && memcmp(c, want, (n + m) * sizeof *c) != 0)) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
    free(c);
}

// C has no empty array, so an empty list is the array {0} with a count of 0.
#define CHECK(name, n, a, m, b, want) \
    do {                              \
        const int x[] = a;            \
        const int y[] = b;            \
        const int w[] = want;         \
        check(name, x, n, y, m, w);   \
    } while (0)

int main(void) {
    CHECK("two-pairs", 2, L(1, 2), 2, L(3, 4), L(1, 2, 3, 4));
    CHECK("uneven", 1, L(5), 3, L(6, 7, 8), L(5, 6, 7, 8));
    CHECK("both-empty", 0, L(0), 0, L(0), L(0));
    CHECK("left-empty", 0, L(0), 2, L(1, 2), L(1, 2));
    CHECK("right-empty", 2, L(1, 2), 0, L(0), L(1, 2));
    CHECK("duplicates", 2, L(1, 1), 1, L(1), L(1, 1, 1));
    CHECK("order", 2, L(3, 1), 1, L(2), L(3, 1, 2));
    CHECK("limit", 1, L(INT_MAX), 1, L(INT_MIN), L(INT_MAX, INT_MIN));
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
