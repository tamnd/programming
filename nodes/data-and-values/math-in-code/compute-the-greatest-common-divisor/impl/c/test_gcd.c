// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-greatest-common-divisor/.

#include "gcd.h"

#include <stdio.h>
#include <stdlib.h>

struct test_case {
    const char *name;
    long long a, b, gcd, lcm;
};

static const struct test_case cases[] = {
    {"common-factor", 12, 18, 6, 36},
    {"coprime", 17, 5, 1, 85},
    {"euclid-example", 48, 180, 12, 720},
    {"zero-and-number", 0, 7, 7, 0},
    {"both-zero", 0, 0, 0, 0},
    {"negative", -12, 18, 6, 36},
    {"both-negative", -4, -6, 2, 12},
    {"equal", 9, 9, 9, 9},
    {"one", 1, 1000000007, 1, 1000000007},
    {"multiple", 5, 25, 5, 25},
    {"fibonacci", 832040, 514229, 1, 427859097160LL},
    {"big-lcm", 3000000000000000000LL, 6000000000000000000LL, 3000000000000000000LL, 6000000000000000000LL},
    {"largest", 9223372036854775807LL, -9223372036854775807LL, 9223372036854775807LL, 9223372036854775807LL},
};

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        long long g1, l1, g2, l2;
        gcd_and_lcm(c->a, c->b, &g1, &l1);
        gcd_and_lcm(c->b, c->a, &g2, &l2);
        if (g1 != c->gcd || l1 != c->lcm || g2 != c->gcd || l2 != c->lcm) {
            printf("FAIL %s: (%lld, %lld)\n", c->name, g1, l1);
            failures++;
        }
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
