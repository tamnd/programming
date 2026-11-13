// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/flatten-a-nested-list/.

#include "flatten.h"

#include <stdint.h>
#include <stdio.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// An empty list of lists has {NULL, 0} in it, because C has no empty array.
// N and M give the real lengths.
#define CHECK(name, n, given, m, flat)                                         \
    do {                                                                       \
        const struct int_list g[] = given;                                     \
        const int32_t want[] = flat;                                           \
        const size_t want_n = m;                                               \
        int32_t out[sizeof want / sizeof want[0]];                             \
        size_t len = 0;                                                        \
        int ok = flatten(g, n, out, want_n, &len) == 0 && len == want_n;       \
        for (size_t i = 0; ok && i < want_n; i++) {                            \
            ok = out[i] == want[i];                                            \
        }                                                                      \
        if (!ok) {                                                             \
            fprintf(stderr, "case %s failed\n", name);                         \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("three", 3, L({(const int32_t[]){1, 2}, 2}, {(const int32_t[]){3}, 1}, {(const int32_t[]){4, 5, 6}, 3}), 6, L(1, 2, 3, 4, 5, 6));
    CHECK("two", 2, L({(const int32_t[]){7}, 1}, {(const int32_t[]){8, 9}, 2}), 3, L(7, 8, 9));
    CHECK("empty", 0, L({NULL, 0}), 0, L(0));
    CHECK("empty-inner", 3, L({NULL, 0}, {(const int32_t[]){1}, 1}, {NULL, 0}), 1, L(1));
    CHECK("all-empty", 2, L({NULL, 0}, {NULL, 0}), 0, L(0));
    CHECK("single", 1, L({(const int32_t[]){5}, 1}), 1, L(5));
    CHECK("one-list", 1, L({(const int32_t[]){1, 2, 3}, 3}), 3, L(1, 2, 3));
    CHECK("negative", 2, L({(const int32_t[]){-1, -2}, 2}, {(const int32_t[]){3}, 1}), 3, L(-1, -2, 3));
    CHECK("extremes", 2, L({(const int32_t[]){INT32_MIN}, 1}, {(const int32_t[]){2147483647, 0}, 2}), 3, L(INT32_MIN, 2147483647, 0));

    // out has room for 2 items, and there are 3.
    const struct int_list three[] = {{(const int32_t[]){1, 2}, 2}, {(const int32_t[]){3}, 1}};
    int32_t small[2] = {0, 0};
    size_t len = 0;
    if (flatten(three, 2, small, 2, &len) != -1 || small[0] != 0) {
        fprintf(stderr, "case small failed\n");
        failures++;
    }

    // The sum of the lengths wraps around, so flatten must give -1.
    const struct int_list huge[] = {{NULL, SIZE_MAX}, {NULL, 2}};
    if (flatten(huge, 2, small, 2, &len) != -1) {
        fprintf(stderr, "case wrap failed\n");
        failures++;
    }

    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
