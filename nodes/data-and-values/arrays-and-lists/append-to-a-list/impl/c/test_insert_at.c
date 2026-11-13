// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-list/.

#include "insert_at.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

static void check(const char *name, const int *items, size_t n, size_t index, int value, const int *want) {
    int *out = insert_at(items, n, index, value);
    if (out == NULL || memcmp(out, want, (n + 1) * sizeof *out) != 0) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
    free(out);
}

// C has no empty array, so an empty list is the array {0} with a count of 0.
#define CHECK(name, n, items, index, value, want) \
    do {                                          \
        const int x[] = items;                    \
        const int w[] = want;                     \
        check(name, x, n, index, value, w);       \
    } while (0)

int main(void) {
    CHECK("at-end", 2, L(1, 2), 2, 3, L(1, 2, 3));
    CHECK("at-front", 2, L(2, 3), 0, 1, L(1, 2, 3));
    CHECK("in-middle", 2, L(1, 3), 1, 2, L(1, 2, 3));
    CHECK("empty", 0, L(0), 0, 7, L(7));
    CHECK("single-front", 1, L(5), 0, 4, L(4, 5));
    CHECK("single-end", 1, L(5), 1, 6, L(5, 6));
    CHECK("end-of-many", 4, L(1, 2, 3, 4), 4, 5, L(1, 2, 3, 4, 5));
    CHECK("front-of-many", 4, L(1, 2, 3, 4), 0, 0, L(0, 1, 2, 3, 4));
    CHECK("duplicates", 2, L(1, 1), 1, 1, L(1, 1, 1));
    CHECK("limit", 1, L(INT_MAX), 1, INT_MIN, L(INT_MAX, INT_MIN));
    const int two[] = {1, 2};
    int *bad = insert_at(two, 2, 3, 9);
    if (bad != NULL) {
        fprintf(stderr, "case index-past-the-end failed\n");
        free(bad);
        failures++;
    }
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
