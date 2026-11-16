// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/return-multiple-values/.

#include "divmod.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

struct test_case {
    const char *name;
    int a, b, q, r;
};

static const struct test_case cases[] = {
    {"positive", 17, 5, 3, 2},
    {"exact", 20, 4, 5, 0},
    {"smaller-dividend", 3, 10, 0, 3},
    {"negative-dividend", -17, 5, -4, 3},
    {"negative-divisor", 17, -5, -4, -3},
    {"both-negative", -17, -5, 3, -2},
    {"exact-negative", -20, 4, -5, 0},
    {"small-negative", -3, 10, -1, 7},
    {"zero-dividend", 0, 7, 0, 0},
    {"zero-negative-divisor", 0, -7, 0, 0},
    {"divisor-one", 9, 1, 9, 0},
    {"int-max", INT_MAX, 2, 1073741823, 1},
    {"int-min", INT_MIN, 3, -715827883, 1},
    {"min-by-max", INT_MIN, INT_MAX, -2, 2147483646},
    {"max-by-min", INT_MAX, INT_MIN, -1, -1},
};

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        int q, r;
        div_mod(c->a, c->b, &q, &r);
        if (q != c->q || r != c->r) {
            printf("FAIL %s: got (%d, %d), want (%d, %d)\n", c->name, q, r, c->q, c->r);
            failures++;
        }
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
