// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/partition-a-list/.

#include "partition_odd.h"

#include <stdbool.h>
#include <stdio.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

static bool same(const int *got, size_t k, const int *want, size_t m) {
    if (k != m) {
        return false;
    }
    for (size_t i = 0; i < m; i++) {
        if (got[i] != want[i]) {
            return false;
        }
    }
    return true;
}

// Each list has a 0 in it when it is empty, because C has no empty array.
// The counts give the real lengths.
#define CHECK(name, n, given, mo, wo, me, we)                                  \
    do {                                                                       \
        const int g[] = given;                                                 \
        const int o[] = wo;                                                    \
        const int e[] = we;                                                    \
        int odd[sizeof g / sizeof g[0]];                                       \
        int even[sizeof g / sizeof g[0]];                                      \
        size_t no = 0;                                                         \
        size_t ne = 0;                                                         \
        partition_odd(g, n, odd, &no, even, &ne);                              \
        if (!same(odd, no, o, mo) || !same(even, ne, e, me)) {                 \
            fprintf(stderr, "case %s failed\n", name);                         \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("mixed", 5, L(1, 2, 3, 4, 5), 3, L(1, 3, 5), 2, L(2, 4));
    CHECK("tens", 4, L(10, 15, 20, 25), 2, L(15, 25), 2, L(10, 20));
    CHECK("empty", 0, L(0), 0, L(0), 0, L(0));
    CHECK("all-odd", 2, L(1, 3), 2, L(1, 3), 0, L(0));
    CHECK("all-even", 2, L(2, 4), 0, L(0), 2, L(2, 4));
    CHECK("negative", 4, L(-3, -2, -1, 0), 2, L(-3, -1), 2, L(-2, 0));
    CHECK("zero", 1, L(0), 0, L(0), 1, L(0));
    CHECK("order", 4, L(9, 8, 1, 2), 2, L(9, 1), 2, L(8, 2));
    CHECK("extremes", 2, L((-2147483647 - 1), 2147483647), 1, L(2147483647), 1, L((-2147483647 - 1)));
    CHECK("duplicates", 4, L(3, 3, 2, 2), 2, L(3, 3), 2, L(2, 2));
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
