// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-integer-overflow/.

#include "checked_add.h"

#include <stdio.h>

static int failures = 0;

static void check(const char *label, int64_t a, int64_t b, bool want_ok, int64_t want) {
    int64_t got = 0;
    bool ok = checked_add(a, b, &got);
    if (ok != want_ok || (ok && got != want)) {
        fprintf(stderr, "case %s failed\n", label);
        failures++;
    }
    got = 0;
    ok = checked_add_portable(a, b, &got);
    if (ok != want_ok || (ok && got != want)) {
        fprintf(stderr, "case %s (portable) failed\n", label);
        failures++;
    }
}

int main(void) {
    check("small", 2, 3, true, 5);
    check("negative", -7, 4, true, -3);
    check("beyond-32-bit", 4000000000, 5000000000, true, 9000000000);
    check("max-plus-zero", INT64_MAX, 0, true, INT64_MAX);
    check("max-plus-one", INT64_MAX, 1, false, 0);
    check("max-plus-max", INT64_MAX, INT64_MAX, false, 0);
    check("min-plus-zero", INT64_MIN, 0, true, INT64_MIN);
    check("min-minus-one", INT64_MIN, -1, false, 0);
    check("min-plus-min", INT64_MIN, INT64_MIN, false, 0);
    check("min-plus-max", INT64_MIN, INT64_MAX, true, -1);
    check("max-minus-one", INT64_MAX, -1, true, INT64_MAX - 1);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
