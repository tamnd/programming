// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-queue/.

#include "round_robin.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

#define CHECK(label, w, s, want)                                               \
    do {                                                                       \
        const int32_t work[] = w;                                              \
        const size_t expect[] = want;                                          \
        const size_t n = sizeof work / sizeof work[0];                         \
        size_t order[sizeof work / sizeof work[0]] = {0};                      \
        if (!finish_order(work, n, (s), order) ||                              \
            memcmp(order, expect, sizeof expect) != 0) {                       \
            fprintf(stderr, "case %s failed\n", label);                        \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("example", L(3, 1, 2), 1, L(1, 2, 0));
    CHECK("slice-two", L(4, 1, 3, 2), 2, L(1, 3, 0, 2));
    size_t none[1] = {0};
    if (!finish_order(NULL, 0, 3, none)) { // the case "empty"
        fprintf(stderr, "case empty failed\n");
        failures++;
    }
    CHECK("one", L(7), 2, L(0));
    CHECK("big-slice", L(3, 1, 2), 10, L(0, 1, 2));
    CHECK("slice-one", L(2, 2, 1), 1, L(2, 0, 1));
    CHECK("equal-work", L(2, 2, 2), 1, L(0, 1, 2));
    CHECK("long-job-first", L(10, 1, 1), 3, L(1, 2, 0));
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
