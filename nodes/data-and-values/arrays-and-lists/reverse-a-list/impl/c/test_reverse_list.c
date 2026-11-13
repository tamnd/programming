// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-list/.

#include "reverse_list.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

static void check(const char *name, const int *items, size_t n, const int *want) {
    int *out = reverse_list(items, n);
    if (out == NULL || (n > 0 && memcmp(out, want, n * sizeof *out) != 0)) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
    free(out);
}

// C has no empty array, so an empty list is the array {0} with a count of 0.
#define CHECK(name, n, items, want) \
    do {                            \
        const int x[] = items;      \
        const int w[] = want;       \
        check(name, x, n, w);       \
    } while (0)

int main(void) {
    CHECK("three", 3, L(1, 2, 3), L(3, 2, 1));
    CHECK("four", 4, L(4, 7, 1, 9), L(9, 1, 7, 4));
    CHECK("empty", 0, L(0), L(0));
    CHECK("single", 1, L(5), L(5));
    CHECK("two", 2, L(1, 2), L(2, 1));
    CHECK("duplicates", 3, L(1, 1, 2), L(2, 1, 1));
    CHECK("palindrome", 3, L(1, 2, 1), L(1, 2, 1));
    CHECK("negative", 3, L(-1, 0, -3), L(-3, 0, -1));
    CHECK("limit", 3, L(INT_MAX, INT_MIN, 0), L(0, INT_MIN, INT_MAX));
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
