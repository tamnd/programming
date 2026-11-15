// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-class/.

#include "rectangle.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, int width, int height, int factor, int a, int b, int c, int d) {
    const int want[4] = {a, b, c, d};
    int got[4];
    rectangle_report(width, height, factor, got);
    if (memcmp(got, want, sizeof want) != 0) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
}

int main(void) {
    check("three-by-four", 3, 4, 2, 12, 14, 48, 28);
    check("square", 5, 5, 3, 25, 20, 225, 60);
    check("zero-side", 0, 7, 2, 0, 14, 0, 28);
    check("zero-factor", 3, 4, 0, 12, 14, 0, 0);
    check("identity", 3, 4, 1, 12, 14, 12, 14);
    check("limit", 1000, 1000, 46, 1000000, 4000, 2116000000, 184000);

    // Each struct value has its own fields.
    struct rectangle a = rectangle_make(1, 2);
    struct rectangle b = rectangle_make(1, 2);
    rectangle_scale(&a, 3);
    if (a.width != 3 || a.height != 6 || b.width != 1 || b.height != 2) {
        fprintf(stderr, "case own-fields failed\n");
        failures++;
    }
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
