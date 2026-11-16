// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-floating-point-numbers/.

#include "is_close.h"

#include <float.h>
#include <math.h>
#include <stdio.h>

struct test_case {
    const char *name;
    double a, b, rel_tol, abs_tol;
    bool want;
};

static const struct test_case cases[] = {
    {"equal", 1.5, 1.5, 1e-9, 0.0, true},
    {"sum-of-tenths", 0.30000000000000004, 0.3, 1e-9, 0.0, true},
    {"different", 1.0, 1.1, 1e-9, 0.0, false},
    {"large-values", 10000000000.0, 10000000001.0, 1e-9, 0.0, true},
    {"zero-and-zero", 0.0, 0.0, 0.0, 0.0, true},
    {"one-ulp-apart", 1.0, 1.0000000000000002, 0.0, 0.0, false},
    {"near-zero-relative", 1e-12, 0.0, 1e-9, 0.0, false},
    {"near-zero-absolute", 1e-12, 0.0, 1e-9, 1e-9, true},
    {"opposite-signs", 1e-10, -1e-10, 1e-9, 0.0, false},
    {"opposite-signs-absolute", 1e-10, -1e-10, 1e-9, 1e-9, true},
    {"on-the-boundary", 10.0, 9.0, 0.1, 0.0, true},
    {"swapped-boundary", 9.0, 10.0, 0.1, 0.0, true},
    {"past-the-boundary", 10.0, 8.875, 0.1, 0.0, false},
    {"negative-values", -1.0, -1.0000000001, 1e-9, 0.0, true},
};

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        if (is_close(c->a, c->b, c->rel_tol, c->abs_tol) != c->want ||
            is_close(c->b, c->a, c->rel_tol, c->abs_tol) != c->want) {
            fprintf(stderr, "case %s failed\n", c->name);
            failures++;
        }
    }
    if (is_close(NAN, NAN, 1e-9, 0.0) || !is_close(INFINITY, INFINITY, 1e-9, 0.0) ||
        is_close(INFINITY, -INFINITY, 1e-9, 0.0) || is_close(INFINITY, 1e308, 1e-9, 0.0)) {
        fprintf(stderr, "non-finite values failed\n");
        failures++;
    }
    if (nextafter(1.0, 2.0) - 1.0 != DBL_EPSILON) {
        fprintf(stderr, "DBL_EPSILON failed\n");
        failures++;
    }
    if (failures != 0) {
        return 1;
    }
    puts("ok");
    return 0;
}
