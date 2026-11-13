// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/map-over-a-list/.

#include "square_all.h"

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
        square_all(g, n, out); \
        size_t k = n; \
        check(name, out, k, w, m);      \
    } while (0)

int main(void) {
    CHECK("small", 3, L(1, 2, 3), 3, L(1, 4, 9));
    CHECK("tens", 2, L(5, 10), 2, L(25, 100));
    CHECK("empty", 0, L(0), 0, L(0));
    CHECK("single", 1, L(7), 1, L(49));
    CHECK("negative", 2, L(-3, -1), 2, L(9, 1));
    CHECK("zero", 1, L(0), 1, L(0));
    CHECK("limit", 2, L(46340, -46340), 2, L(2147395600, 2147395600));
    CHECK("order", 3, L(3, 1, 2), 3, L(9, 1, 4));
    CHECK("duplicates", 2, L(2, 2), 2, L(4, 4));
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
