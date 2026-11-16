// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-fibonacci-numbers/.

#include "fibonacci.h"

#include <stdio.h>
#include <stdlib.h>

struct test_case {
    const char *name;
    int n;
    long long want;
};

static const struct test_case cases[] = {
    {"ten", 10, 55},
    {"twenty", 20, 6765},
    {"thirty", 30, 832040},
    {"zero", 0, 0},
    {"one", 1, 1},
    {"two", 2, 1},
    {"largest-32-bit", 46, 1836311903},
    {"beyond-32-bit", 47, 2971215073LL},
    {"binet-fails", 71, 308061521170129LL},
    {"largest-64-bit", 92, 7540113804746346429LL},
};

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        long long got = fibonacci(c->n);
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
