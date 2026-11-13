// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/slice-a-list/.

#include "slice_list.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

static void check(const char *name, const int *items, size_t n, size_t start, size_t stop, const int *want) {
    int *out = slice_list(items, n, start, stop);
    if (out == NULL || (stop > start && memcmp(out, want, (stop - start) * sizeof *out) != 0)) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
    free(out);
}

// C has no empty array, so an empty list is the array {0} with a count of 0.
#define CHECK(name, n, items, start, stop, want) \
    do {                                         \
        const int x[] = items;                   \
        const int w[] = want;                    \
        check(name, x, n, start, stop, w);       \
    } while (0)

int main(void) {
    CHECK("middle", 5, L(1, 2, 3, 4, 5), 1, 3, L(2, 3));
    CHECK("prefix", 3, L(1, 2, 3), 0, 2, L(1, 2));
    CHECK("suffix", 3, L(1, 2, 3), 1, 3, L(2, 3));
    CHECK("empty-list", 0, L(0), 0, 0, L(0));
    CHECK("empty-slice", 3, L(1, 2, 3), 2, 2, L(0));
    CHECK("whole", 3, L(1, 2, 3), 0, 3, L(1, 2, 3));
    CHECK("single", 1, L(7), 0, 1, L(7));
    CHECK("last-one", 3, L(1, 2, 3), 2, 3, L(3));
    CHECK("at-end", 3, L(1, 2, 3), 3, 3, L(0));
    CHECK("limit", 3, L(INT_MAX, INT_MIN, 0), 0, 2, L(INT_MAX, INT_MIN));
    const int three[] = {1, 2, 3};
    if (slice_list(three, 3, 0, 4) != NULL || slice_list(three, 3, 2, 1) != NULL) {
        fprintf(stderr, "case bad-range failed\n");
        failures++;
    }
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
