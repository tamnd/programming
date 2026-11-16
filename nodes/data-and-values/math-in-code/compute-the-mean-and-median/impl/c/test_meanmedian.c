// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-mean-and-median/.

#include "meanmedian.h"

#include <stdio.h>
#include <stdlib.h>

struct test_case {
    const char *name;
    long long items[5];
    size_t n;
    double mean, median;
};

static const struct test_case cases[] = {
    {"odd", {3, 1, 2}, 3, 2.0, 2.0},
    {"even", {4, 1, 3, 2}, 4, 2.5, 2.5},
    {"skewed", {1, 2, 3, 4, 100}, 5, 22.0, 3.0},
    {"single", {5}, 1, 5.0, 5.0},
    {"two-items", {1, 2}, 2, 1.5, 1.5},
    {"negative", {-3, -1, -2}, 3, -2.0, -2.0},
    {"unsorted-even", {10, 2, 8, 4}, 4, 6.0, 6.0},
    {"duplicates", {1, 1, 1, 5}, 4, 2.0, 1.0},
    {"not-an-integer", {1, 2, 2}, 3, 1.6666666666666667, 2.0},
    {"large", {9223372036854775807LL, 9223372036854775807LL}, 2, 9223372036854775808.0, 9223372036854775808.0},
};

// The test does not use fabs, so it needs no math library.
static bool close_to(double got, double want) {
    double d = got - want;
    double w = want < 0 ? -want : want;
    double tol = 1e-12 * w > 1e-9 ? 1e-12 * w : 1e-9;
    return (d < 0 ? -d : d) <= tol;
}

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        double mean, median;
        if (!mean_and_median(c->items, c->n, &mean, &median) || !close_to(mean, c->mean) ||
            !close_to(median, c->median)) {
            printf("FAIL %s\n", c->name);
            failures++;
        }
    }
    double mean, median;
    if (mean_and_median(cases[0].items, 0, &mean, &median)) {
        printf("FAIL empty\n");
        failures++;
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
