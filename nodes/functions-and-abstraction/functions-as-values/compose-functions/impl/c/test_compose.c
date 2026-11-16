// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compose-functions/.

#include "compose.h"

#include <stdio.h>
#include <stdlib.h>

#define D5 "double", "double", "double", "double", "double"
#define D20 D5, D5, D5, D5
#define DI5 "double", "inc", "double", "inc", "double", "inc", "double", "inc", "double", "inc"
#define ID5 "inc", "double", "inc", "double", "inc", "double", "inc", "double", "inc", "double"

struct test_case {
    const char *name;
    const char *ops[COMPOSE_MAX];
    size_t n;
    int x, want;
};

static const struct test_case cases[] = {
    {"inc-after-double", {"inc", "double"}, 2, 3, 7},
    {"double-after-inc", {"double", "inc"}, 2, 3, 8},
    {"three-functions", {"negate", "dec", "double"}, 3, 5, -9},
    {"empty", {NULL}, 0, 42, 42},
    {"single-zero", {"negate"}, 1, 0, 0},
    {"negative", {"dec", "negate"}, 2, -5, 4},
    {"low", {"inc"}, 1, -1000, -999},
    {"duplicates", {"inc", "inc", "inc"}, 3, 7, 10},
    {"limit-high", {D20}, 20, 1000, 1048576000},
    {"limit-low", {D20}, 20, -1000, -1048576000},
    {"mixed-20", {DI5, DI5}, 20, 1000, 1026046},
    {"mixed-20-swapped", {ID5, ID5}, 20, 1000, 1025023},
};

static int square(int x) {
    return x * x;
}

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        int got = compose_all(c->ops, c->n, c->x);
        if (got != c->want) {
            printf("FAIL %s: got %d, want %d\n", c->name, got, c->want);
            failures++;
        }
    }
    const int_fn fs[] = {square, op_by_name("inc")};
    int got = apply_composed(fs, 2, 4);
    if (got != 25) {
        printf("FAIL square after inc: got %d, want 25\n", got);
        failures++;
    }
    if (op_by_name("halve") != NULL) {
        printf("FAIL an unknown name gives a function\n");
        failures++;
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
