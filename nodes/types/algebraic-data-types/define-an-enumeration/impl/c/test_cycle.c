// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-an-enumeration/.

#include "cycle.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void check(const char *label, const char *start, size_t steps, const char *const *want, size_t want_n) {
    const char *out[16];
    size_t n = cycle(start, steps, out);
    int ok = n == want_n;
    for (size_t i = 0; ok && i < n; i++) {
        ok = strcmp(out[i], want[i]) == 0;
    }
    if (!ok) {
        fprintf(stderr, "case %s failed\n", label);
        failures++;
    }
}

#define CHECK(label, start, steps, ...)                                        \
    do {                                                                       \
        const char *const want[] = {__VA_ARGS__};                              \
        check(label, start, steps, want, sizeof want / sizeof want[0] - 1);    \
    } while (0)

int main(void) {
    // Each list of names ends with a dummy "", because C has no empty array.
    CHECK("weekdays", "monday", 3, "monday", "tuesday", "wednesday", "");
    CHECK("wrap", "friday", 4, "friday", "saturday", "sunday", "monday", "");
    CHECK("zero-steps", "monday", 0, "");
    CHECK("more-than-a-week", "sunday", 9, "sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday", "monday", "");
    CHECK("unknown", "funday", 3, "");
    CHECK("wrong-case", "Friday", 2, "");
    CHECK("number", "3", 2, "");
    CHECK("empty-name", "", 1, "");
    if (FRIDAY != 4 || strcmp(day_name(FRIDAY), "friday") != 0 || day_name((enum day)9) != NULL) {
        fprintf(stderr, "case day_name failed\n");
        failures++;
    }
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
