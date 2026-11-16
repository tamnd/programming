// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/swap-two-values/.

#include "swap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct test_case {
    const char *name;
    size_t n;
    long long xs[8];
    size_t i, j;
    long long want[8];
};

static const struct test_case cases[] = {
    {"first-and-last", 3, {1, 2, 3}, 0, 2, {3, 2, 1}},
    {"middle-pair", 4, {10, 20, 30, 40}, 1, 2, {10, 30, 20, 40}},
    {"two-items", 2, {5, 7}, 0, 1, {7, 5}},
    {"single-item", 1, {9}, 0, 0, {9}},
    {"same-index", 3, {1, 2, 3}, 1, 1, {1, 2, 3}},
    {"higher-index-first", 4, {1, 2, 3, 4}, 3, 0, {4, 2, 3, 1}},
    {"equal-values", 2, {4, 4}, 0, 1, {4, 4}},
    {"negative-values", 3, {-1, 0, 1}, 0, 2, {1, 0, -1}},
    {"int32-extremes", 2, {2147483647, -2147483648LL}, 0, 1, {-2147483648LL, 2147483647}},
    {"eight-items", 8, {1, 2, 3, 4, 5, 6, 7, 8}, 2, 6, {1, 2, 7, 4, 5, 6, 3, 8}},
};

int main(void) {
    int failures = 0;
    for (size_t k = 0; k < sizeof cases / sizeof cases[0]; k++) {
        const struct test_case *c = &cases[k];
        long long buf[8];
        memcpy(buf, c->xs, sizeof buf);
        swap_items(buf, c->i, c->j);
        if (memcmp(buf, c->want, c->n * sizeof buf[0]) != 0) {
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
