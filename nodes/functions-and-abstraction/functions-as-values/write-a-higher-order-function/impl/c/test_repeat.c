// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-higher-order-function/.

#include "repeat.h"

#include <stdio.h>
#include <stdlib.h>

struct test_case {
    const char *name;
    int start, times, want;
};

static const struct test_case cases[] = {
    {"one-step", 6, 1, 3},
    {"three-steps", 6, 3, 5},
    {"ten-steps", 27, 10, 214},
    {"zero-times", 7, 0, 7},
    {"start-one", 1, 1, 4},
    {"cycle", 1, 3, 1},
    {"past-one", 16, 10, 1},
    {"peak-27", 27, 77, 9232},
    {"peak-9663", 9663, 48, 27114424},
    {"limit", 10000, 1000, 2},
};

static int twice(int x) {
    return x * 2;
}

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        int got = collatz_after(c->start, c->times);
        if (got != c->want) {
            printf("FAIL %s: got %d, want %d\n", c->name, got, c->want);
            failures++;
        }
    }
    int doubled = call_repeated(repeat(twice, 10), 1);
    if (doubled != 1024) {
        printf("FAIL repeat(twice, 10): got %d, want 1024\n", doubled);
        failures++;
    }
    int same = call_repeated(repeat(collatz_step, 0), 27);
    if (same != 27) {
        printf("FAIL repeat(collatz_step, 0): got %d, want 27\n", same);
        failures++;
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
