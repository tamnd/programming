// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-variable/.

#include "total.h"

#include <stdio.h>
#include <stdlib.h>

struct test_case {
    const char *name;
    size_t n;
    long long values[8];
    long long want;
};

static const struct test_case cases[] = {
    {"one-two-three", 3, {1, 2, 3}, 6},
    {"four-values", 4, {10, 20, 30, 40}, 100},
    {"mixed-signs", 3, {5, -2, 7}, 10},
    {"empty", 0, {0}, 0},
    {"single", 1, {42}, 42},
    {"all-negative", 3, {-1, -2, -3}, -6},
    {"zeros", 3, {0, 0, 0}, 0},
    {"cancel-out", 2, {7, -7}, 0},
    {"past-32-bits", 2, {4000000000LL, 5000000000LL}, 9000000000LL},
    {"eight-values", 8, {1, 1, 1, 1, 1, 1, 1, 1}, 8},
};

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        long long got = total(c->values, c->n);
        if (got != c->want) {
            printf("FAIL %s: got %lld\n", c->name, got);
            failures++;
        }
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
