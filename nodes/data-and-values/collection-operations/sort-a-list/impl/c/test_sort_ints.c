// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list/.

#include "sort_ints.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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
        memcpy(out, g, sizeof g); \
        sort_ints(out, n); \
        size_t k = n; \
        check(name, out, k, w, m);      \
    } while (0)

int main(void) {
    CHECK("small", 3, L(3, 1, 2), 3, L(1, 2, 3));
    CHECK("five", 5, L(5, 4, 1, 3, 2), 5, L(1, 2, 3, 4, 5));
    CHECK("empty", 0, L(0), 0, L(0));
    CHECK("single", 1, L(1), 1, L(1));
    CHECK("sorted", 3, L(1, 2, 3), 3, L(1, 2, 3));
    CHECK("reversed", 3, L(9, 8, 7), 3, L(7, 8, 9));
    CHECK("duplicates", 4, L(2, 1, 2, 1), 4, L(1, 1, 2, 2));
    CHECK("negative", 3, L(-1, -10, 5), 3, L(-10, -1, 5));
    CHECK("digits", 4, L(10, 9, 100, 1), 4, L(1, 9, 10, 100));
    CHECK("extremes", 3, L(2147483647, (-2147483647 - 1), 0), 3, L((-2147483647 - 1), 0, 2147483647));
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
