// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-matrix-arithmetic/.

#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct test_case {
    const char *name;
    size_t n, m, p;
    long long a[16], b[16], want[16];
};

static const struct test_case cases[] = {
    {"two-by-two", 2, 2, 2, {1, 2, 3, 4}, {5, 6, 7, 8}, {19, 22, 43, 50}},
    {"two-by-three-times-three-by-two", 2, 3, 2, {1, 2, 3, 4, 5, 6}, {7, 8, 9, 10, 11, 12}, {58, 64, 139, 154}},
    {"one-by-one", 1, 1, 1, {3}, {4}, {12}},
    {"row-times-column", 1, 3, 1, {1, 2, 3}, {4, 5, 6}, {32}},
    {"column-times-row", 3, 1, 3, {1, 2, 3}, {4, 5, 6}, {4, 5, 6, 8, 10, 12, 12, 15, 18}},
    {"identity", 2, 2, 2, {1, 0, 0, 1}, {5, 6, 7, 8}, {5, 6, 7, 8}},
    {"other-order", 2, 2, 2, {5, 6, 7, 8}, {1, 2, 3, 4}, {23, 34, 31, 46}},
    {"zero-matrix", 2, 2, 2, {0, 0, 0, 0}, {1, 2, 3, 4}, {0, 0, 0, 0}},
    {"negative-numbers", 2, 2, 2, {-1, 2, 3, -4}, {2, 0, 1, -1}, {0, -2, 2, 4}},
    {"beyond-32-bit", 2, 2, 1, {100000, 0, 0, 100000}, {100000, 3}, {10000000000LL, 300000}},
};

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        long long got[16];
        matrix_multiply(c->a, c->b, c->n, c->m, c->p, got);
        if (memcmp(got, c->want, c->n * c->p * sizeof got[0]) != 0) {
            printf("FAIL %s\n", c->name);
            failures++;
        }
    }
    const long long m[] = {1, 2, 3, 4, 5, 6}, want[] = {1, 4, 2, 5, 3, 6};
    long long t[6];
    transpose(m, 2, 3, t);
    if (memcmp(t, want, sizeof t) != 0) {
        printf("FAIL transpose\n");
        failures++;
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
