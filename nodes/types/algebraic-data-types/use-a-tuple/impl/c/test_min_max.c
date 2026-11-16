// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-tuple/.

#include "min_max.h"

#include <stdio.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

#define CHECK(label, nums, lo, hi)                                             \
    do {                                                                       \
        const int32_t numbers[] = nums;                                        \
        const size_t n = sizeof numbers / sizeof numbers[0];                   \
        struct min_max got = min_max(numbers, n);                              \
        if (got.min != (lo) || got.max != (hi)) {                              \
            fprintf(stderr, "case %s failed\n", label);                        \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("mixed", L(3, 1, 2), 1, 3);
    CHECK("longer", L(4, 9, -2, 7, 0), -2, 9);
    CHECK("one", L(5), 5, 5);
    CHECK("negative", L(-3, -1, -7), -7, -1);
    CHECK("all-same", L(2, 2, 2), 2, 2);
    CHECK("int32-extremes", L(INT32_MAX, 0, INT32_MIN), INT32_MIN, INT32_MAX);
    CHECK("sorted", L(1, 2, 3, 4), 1, 4);
    CHECK("reversed", L(4, 3, 2, 1), 1, 4);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
