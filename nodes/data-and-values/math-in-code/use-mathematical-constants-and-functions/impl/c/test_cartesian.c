// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-mathematical-constants-and-functions/.

#include "cartesian.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct test_case {
    const char *name;
    double r, degrees, x, y;
};

static const struct test_case cases[] = {
    {"east", 1.0, 0.0, 1.0, 0.0},
    {"north", 2.0, 90.0, 0.0, 2.0},
    {"west", 2.0, 180.0, -2.0, 0.0},
    {"diagonal", 1.0, 45.0, 0.7071067811865476, 0.7071067811865476},
    {"thirty-degrees", 2.0, 30.0, 1.7320508075688772, 1.0},
    {"zero-radius", 0.0, 123.0, 0.0, 0.0},
    {"south", 1.0, 270.0, 0.0, -1.0},
    {"negative-angle", 1.0, -90.0, 0.0, -1.0},
    {"full-turn", 1.0, 360.0, 1.0, 0.0},
    {"two-turns", 3.0, 720.0, 3.0, 0.0},
    {"negative-radius", -1.0, 0.0, -1.0, 0.0},
    {"large-radius", 1000000.0, 60.0, 500000.0, 866025.4037844386},
};

static int close_to(double got, double want) {
    return fabs(got - want) <= fmax(1e-9, 1e-12 * fabs(want));
}

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        double x, y;
        to_cartesian(c->r, c->degrees, &x, &y);
        if (!close_to(x, c->x) || !close_to(y, c->y)) {
            printf("FAIL %s: (%.17g, %.17g)\n", c->name, x, y);
            failures++;
        }
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
