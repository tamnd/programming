// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-items-of-each-kind/.

#include "count_items.h"

#include <stdio.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// An empty list has a 0 in it, because C has no empty array. The counts give
// the real lengths.
#define CHECK(name, n, given, m, ...)                                          \
    do {                                                                       \
        const int g[] = given;                                                 \
        const struct item_count w[] = {__VA_ARGS__};                           \
        struct item_count out[sizeof g / sizeof g[0]];                         \
        const size_t want_n = m;                                               \
        size_t k = count_items(g, n, out);                                     \
        int ok = k == want_n;                                                  \
        for (size_t i = 0; ok && i < want_n; i++) {                            \
            ok = out[i].item == w[i].item && out[i].count == w[i].count;       \
        }                                                                      \
        if (!ok) {                                                             \
            fprintf(stderr, "case %s failed\n", name);                         \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("mixed", 6, L(3, 1, 3, 2, 1, 3), 3, {1, 2}, {2, 1}, {3, 3});
    CHECK("same", 3, L(5, 5, 5), 1, {5, 3});
    CHECK("empty", 0, L(0), 0, {0, 0});
    CHECK("single", 1, L(7), 1, {7, 1});
    CHECK("negative", 4, L(-1, 2, -1, 0), 3, {-1, 2}, {0, 1}, {2, 1});
    CHECK("distinct", 3, L(4, 2, 9), 3, {2, 1}, {4, 1}, {9, 1});
    CHECK("order", 3, L(9, 8, 9), 2, {8, 1}, {9, 2});
    CHECK("zero", 2, L(0, 0), 1, {0, 2});
    CHECK("extremes", 3, L(2147483647, (-2147483647 - 1), 2147483647), 2, {(-2147483647 - 1), 1}, {2147483647, 2});
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
