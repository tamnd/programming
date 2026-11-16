// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-division-by-zero/.

#include "safe_div.h"

#include <stdio.h>

static int failures = 0;

static void check(const char *label, int64_t a, int64_t b, bool want_ok, int64_t want) {
    int64_t got = 0;
    bool ok = safe_div(a, b, &got);
    if (ok != want_ok || (ok && got != want)) {
        fprintf(stderr, "case %s failed\n", label);
        failures++;
    }
}

int main(void) {
    check("exact", 12, 4, true, 3);
    check("remainder-dropped", 7, 2, true, 3);
    check("zero-dividend", 0, 5, true, 0);
    check("zero-divisor", 5, 0, false, 0);
    check("negative-by-zero", -5, 0, false, 0);
    check("zero-by-zero", 0, 0, false, 0);
    check("negative-dividend", -7, 2, true, -3);
    check("negative-divisor", 7, -2, true, -3);
    check("both-negative", -7, -2, true, 3);
    check("min-by-minus-one", INT64_MIN, -1, false, 0);
    check("min-by-one", INT64_MIN, 1, true, INT64_MIN);
    check("max-by-minus-one", INT64_MAX, -1, true, -INT64_MAX);
    check("min-by-max", INT64_MIN, INT64_MAX, true, -1);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
