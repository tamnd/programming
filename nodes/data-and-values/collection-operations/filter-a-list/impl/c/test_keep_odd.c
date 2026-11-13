// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-a-list/.

#include "keep_odd.h"

#include <stdbool.h>
#include <stdio.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// Each list has a 0 at the end when it is empty, because C has no empty
// array. The counts n and m give the real lengths.
static void check(const char *name, const int *got, size_t k, const int *want, size_t m) {
    bool ok = k == m;
    for (size_t i = 0; ok && i < m; i++) {
        ok = got[i] == want[i];
    }
    if (!ok) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
}

#define CHECK(name, n, given, m, want)  \
    do {                                \
        const int g[] = given;          \
        const int w[] = want;           \
        int out[sizeof g / sizeof g[0]]; \
        size_t k = keep_odd(g, n, out); \
        check(name, out, k, w, m);      \
    } while (0)

int main(void) {
    CHECK("mixed", 5, L(1, 2, 3, 4, 5), 3, L(1, 3, 5));
    CHECK("tens", 4, L(10, 15, 20, 25), 2, L(15, 25));
    CHECK("empty", 0, L(0), 0, L(0));
    CHECK("none-odd", 3, L(2, 4, 6), 0, L(0));
    CHECK("all-odd", 3, L(1, 3, 5), 3, L(1, 3, 5));
    CHECK("negative", 4, L(-3, -2, -1, 0), 2, L(-3, -1));
    CHECK("zero", 1, L(0), 0, L(0));
    CHECK("order", 3, L(9, 1, 7), 3, L(9, 1, 7));
    CHECK("extremes", 2, L((-2147483647 - 1), 2147483647), 1, L(2147483647));
    CHECK("duplicates", 4, L(3, 3, 2, 3), 3, L(3, 3, 3));
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
