// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-complex-number-arithmetic/.

#include "complexarith.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct test_case {
    const char *name;
    double a[2], b[2], want[8];
};

static const struct test_case cases[] = {
    {"textbook", {3.0, 4.0}, {1.0, -2.0}, {4.0, 2.0, 11.0, -2.0, -1.0, 2.0, 5.0, 0.9272952180016122}},
    {"rosetta", {1.0, 1.0}, {3.14159, 1.25}, {4.14159, 2.25, 1.8915899999999999, 4.39159, 0.38414593243590134, 0.16546321590504276, 1.4142135623730951, 0.7853981633974483}},
    {"real-only", {6.0, 0.0}, {3.0, 0.0}, {9.0, 0.0, 18.0, 0.0, 2.0, 0.0, 6.0, 0.0}},
    {"imaginary-unit", {0.0, 1.0}, {0.0, 1.0}, {0.0, 2.0, -1.0, 0.0, 1.0, 0.0, 1.0, 1.5707963267948966}},
    {"negative-real", {-2.0, 0.0}, {1.0, 0.0}, {-1.0, 0.0, -2.0, 0.0, -2.0, 0.0, 2.0, 3.141592653589793}},
    {"zero", {0.0, 0.0}, {2.0, -3.0}, {2.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
    {"negative-phase", {1.0, -1.0}, {-1.0, -1.0}, {0.0, -2.0, -2.0, 0.0, 0.0, 1.0, 1.4142135623730951, -0.7853981633974483}},
    {"imaginary-divisor", {2.0, 4.0}, {0.0, 2.0}, {2.0, 6.0, -8.0, 4.0, 2.0, -1.0, 4.47213595499958, 1.1071487177940904}},
    {"large-divisor", {1e+150, 1e+150}, {1e+155, 1e+155}, {1.00001e+155, 1.00001e+155, 0.0, 2e+305, 9.999999999999999e-06, 0.0, 1.414213562373095e+150, 0.7853981633974483}},
};

static int close_to(double got, double want) {
    return fabs(got - want) <= fmax(1e-9, 1e-12 * fabs(want));
}

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        double got[8];
        complex_arithmetic(c->a, c->b, got);
        for (int k = 0; k < 8; k++) {
            if (!close_to(got[k], c->want[k])) {
                printf("FAIL %s: item %d = %.17g\n", c->name, k, got[k]);
                failures++;
            }
        }
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
