// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-a-collection/.

#include "total.h"

#include <limits.h>
#include <stdio.h>

static int failures = 0;

static void check(const char *name, const int *items, size_t n, long long want) {
    long long got = total(items, n);
    if (got != want) {
        fprintf(stderr, "case %s failed: got %lld, want %lld\n", name, got, want);
        failures++;
    }
}

#define CHECK(name, want, ...)                                 \
    do {                                                       \
        const int items[] = {__VA_ARGS__};                     \
        check(name, items, sizeof items / sizeof items[0], want); \
    } while (0)

int main(void) {
    CHECK("three", 6, 1, 2, 3);
    CHECK("mixed", 16, 10, -4, 7, 0, 3);
    check("empty", NULL, 0, 0);
    CHECK("single", 7, 7);
    CHECK("negative", -4, -5, 2, -1);
    CHECK("limit", 4294967296LL, INT_MAX, INT_MAX, 2);
    CHECK("limit-low", -4294967296LL, INT_MIN, INT_MIN);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
