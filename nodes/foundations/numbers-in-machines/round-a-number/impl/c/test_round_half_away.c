// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/round-a-number/.

#include "round_half_away.h"

#include <math.h>
#include <stdio.h>

static int failures = 0;

static void check(const char *label, double x, long long want) {
    long long got = round_half_away(x);
    if (got != want) {
        fprintf(stderr, "case %s: got %lld, want %lld\n", label, got, want);
        failures++;
    }
}

int main(void) {
    check("down", 2.4, 2);
    check("up", 2.6, 3);
    check("negative", -2.6, -3);
    check("integer", 7.0, 7);
    check("half-even-up", 2.5, 3);
    check("half-odd-up", 1.5, 2);
    check("half-zero", 0.5, 1);
    check("half-negative", -2.5, -3);
    check("half-negative-small", -0.5, -1);
    check("negative-below-half", -0.4, 0);
    check("near-half", 0.49999999999999994, 0);
    check("zero", 0.0, 0);
    check("large-odd", 4503599627370497.0, 4503599627370497LL);
    check("large-half-negative", -1000000000000000.5, -1000000000000001LL);
    // rint uses the current rounding mode; the default mode rounds a half to even.
    if (rint(2.5) != 2.0 || floor(-2.5) != -3.0 || ceil(-2.5) != -2.0 || trunc(-2.5) != -2.0) {
        fprintf(stderr, "library modes\n");
        failures++;
    }
    if (failures != 0) {
        return 1;
    }
    puts("ok");
    return 0;
}
