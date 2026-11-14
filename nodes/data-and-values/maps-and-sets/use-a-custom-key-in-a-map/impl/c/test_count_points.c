// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-custom-key-in-a-map/.

#include "count_points.h"

#include <stdio.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// An empty array has a dummy item in it, because C has no empty array. The
// counts give the real lengths.
#define CHECK(name, cnt, pts, kk, ...)                                         \
    do {                                                                       \
        struct point points[] = pts;                                           \
        const struct point_count want[] = {__VA_ARGS__};                       \
        struct point_count out[sizeof points / sizeof points[0]];              \
        const size_t want_k = kk;                                              \
        size_t got = count_points(points, cnt, out);                           \
        int ok = got == want_k;                                                \
        for (size_t i = 0; ok && i < want_k; i++) {                            \
            ok = out[i].p.x == want[i].p.x && out[i].p.y == want[i].p.y &&     \
                 out[i].n == want[i].n;                                        \
        }                                                                      \
        if (!ok) {                                                             \
            fprintf(stderr, "case %s failed\n", name);                         \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("clicks", 3, L({1, 2}, {3, 4}, {1, 2}), 2, {{1, 2}, 2}, {{3, 4}, 1});
    CHECK("swap", 2, L({2, 1}, {1, 2}), 2, {{1, 2}, 1}, {{2, 1}, 1});
    CHECK("empty", 0, L({0, 0}), 0, {{0, 0}, 0});
    CHECK("one", 1, L({0, 0}), 1, {{0, 0}, 1});
    CHECK("negative", 3, L({-1, 0}, {0, -1}, {-1, 0}), 2, {{-1, 0}, 2}, {{0, -1}, 1});
    CHECK("joined-digits", 2, L({1, 23}, {12, 3}), 2, {{1, 23}, 1}, {{12, 3}, 1});
    CHECK("all-same", 3, L({5, 5}, {5, 5}, {5, 5}), 1, {{5, 5}, 3});
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
