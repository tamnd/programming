// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-number-is-prime/.

#include "prime.h"

#include <stdio.h>
#include <stdlib.h>

struct test_case {
    const char *name;
    long long n;
    bool want;
};

static const struct test_case cases[] = {
    {"small-prime", 17, true},
    {"small-composite", 15, false},
    {"even", 100, false},
    {"odd-composite", 91, false},
    {"negative", -7, false},
    {"zero", 0, false},
    {"one", 1, false},
    {"two", 2, true},
    {"three", 3, true},
    {"square-of-prime", 25, false},
    {"large-square", 4293001441LL, false},
    {"carmichael", 561, false},
    {"large-prime", 1000000007, true},
    {"mersenne-prime", 2147483647, true},
    {"large-composite", 2147483649LL, false},
};

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        if (is_prime(c->n) != c->want) {
            printf("FAIL %s\n", c->name);
            failures++;
        }
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
