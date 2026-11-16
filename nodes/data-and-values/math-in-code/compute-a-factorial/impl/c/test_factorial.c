// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-factorial/.

#include "factorial.h"

#include <stdio.h>
#include <stdlib.h>

struct test_case {
    const char *name;
    int n;
    long long want;
};

static const struct test_case cases[] = {
    {"five", 5, 120},
    {"ten", 10, 3628800},
    {"zero", 0, 1},
    {"one", 1, 1},
    {"largest-32-bit", 12, 479001600},
    {"beyond-32-bit", 13, 6227020800LL},
    {"largest-64-bit", 20, 2432902008176640000LL},
};

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        long long got = factorial(c->n);
        if (got != c->want) {
            printf("FAIL %s: %lld\n", c->name, got);
            failures++;
        }
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
