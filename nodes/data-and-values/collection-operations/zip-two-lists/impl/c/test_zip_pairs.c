// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/zip-two-lists/.

#include "zip_pairs.h"

#include <stdio.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// An empty list has a 0 in it, because C has no empty array. The counts give
// the real lengths.
#define CHECK(name, na, ga, nb, gb, m, ...)                                    \
    do {                                                                       \
        const int a[] = ga;                                                    \
        const int b[] = gb;                                                    \
        const struct int_pair w[] = {__VA_ARGS__};                             \
        struct int_pair out[sizeof a / sizeof a[0] + sizeof b / sizeof b[0]];  \
        const size_t want_n = m;                                               \
        size_t k = zip_pairs(a, na, b, nb, out);                               \
        int ok = k == want_n;                                                  \
        for (size_t i = 0; ok && i < want_n; i++) {                            \
            ok = out[i].first == w[i].first && out[i].second == w[i].second;   \
        }                                                                      \
        if (!ok) {                                                             \
            fprintf(stderr, "case %s failed\n", name);                         \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("same-length", 3, L(1, 2, 3), 3, L(4, 5, 6), 3, {1, 4}, {2, 5}, {3, 6});
    CHECK("tens", 2, L(10, 20), 2, L(30, 40), 2, {10, 30}, {20, 40});
    CHECK("empty", 0, L(0), 0, L(0), 0, {0, 0});
    CHECK("first-empty", 0, L(0), 2, L(1, 2), 0, {0, 0});
    CHECK("second-empty", 2, L(1, 2), 0, L(0), 0, {0, 0});
    CHECK("first-shorter", 1, L(1), 2, L(7, 8), 1, {1, 7});
    CHECK("second-shorter", 3, L(1, 2, 3), 1, L(9), 1, {1, 9});
    CHECK("negative", 2, L(-1, -2), 2, L(3, -4), 2, {-1, 3}, {-2, -4});
    CHECK("extremes", 2, L((-2147483647 - 1), 2147483647), 2, L(2147483647, (-2147483647 - 1)), 2, {(-2147483647 - 1), 2147483647}, {2147483647, (-2147483647 - 1)});
    CHECK("duplicates", 2, L(5, 5), 2, L(5, 5), 2, {5, 5}, {5, 5});
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
