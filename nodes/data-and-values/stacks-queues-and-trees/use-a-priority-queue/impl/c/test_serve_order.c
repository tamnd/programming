// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-priority-queue/.

#include "serve_order.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

#define CHECK(label, p, want)                                                  \
    do {                                                                       \
        const int32_t priorities[] = p;                                        \
        const size_t expect[] = want;                                          \
        const size_t n = sizeof priorities / sizeof priorities[0];             \
        size_t order[sizeof priorities / sizeof priorities[0]] = {0};          \
        if (!serve_order(priorities, n, order) ||                              \
            memcmp(order, expect, sizeof expect) != 0) {                       \
            fprintf(stderr, "case %s failed\n", label);                        \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("example", L(2, 5, 1, 5), L(1, 3, 0, 2));
    CHECK("longer", L(3, 1, 4, 1, 5, 9, 2, 6), L(5, 7, 4, 2, 0, 6, 1, 3));
    size_t none[1] = {0};
    if (!serve_order(NULL, 0, none)) { // the case "empty"
        fprintf(stderr, "case empty failed\n");
        failures++;
    }
    CHECK("one", L(7), L(0));
    CHECK("ties", L(4, 4, 4, 4), L(0, 1, 2, 3));
    CHECK("negative", L(-3, -1, -2), L(1, 2, 0));
    CHECK("int32-extremes", L(INT32_MIN, INT32_MAX, 0, INT32_MIN), L(1, 2, 0, 3));
    CHECK("sorted", L(1, 2, 3), L(2, 1, 0));
    CHECK("reversed", L(3, 2, 1), L(0, 1, 2));
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
