// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sum-and-multiply-a-list/.

#include "sumprod.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

struct test_case {
    const char *name;
    long long items[4];
    size_t n;
    long long sum, product;
};

static const struct test_case cases[] = {
    {"small", {1, 2, 3, 4}, 4, 10, 24},
    {"mixed", {3, -2, 5}, 3, 6, -30},
    {"empty", {0}, 0, 0, 1},
    {"single", {7}, 1, 7, 7},
    {"with-zero", {5, 0, 9}, 3, 14, 0},
    {"two-negatives", {-2, -3}, 2, -5, 6},
    {"three-negatives", {-1, -2, -3}, 3, -6, -6},
    {"ones", {1, 1, 1}, 3, 3, 1},
    {"large-product", {3037000499LL, 3037000499LL}, 2, 6074000998LL, 9223372030926249001LL},
    {"to-the-limit", {9223372036854775806LL, 1}, 2, 9223372036854775807LL, 9223372036854775806LL},
    {"beyond-double", {9007199254740993LL}, 1, 9007199254740993LL, 9007199254740993LL},
    {"min", {LLONG_MIN}, 1, LLONG_MIN, LLONG_MIN},
};

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        long long sum, product;
        sum_and_product(c->items, c->n, &sum, &product);
        if (sum != c->sum || product != c->product) {
            printf("FAIL %s: (%lld, %lld)\n", c->name, sum, product);
            failures++;
        }
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
