// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-difference-of-two-dates/.

#include "diff.h"

#include <stdio.h>
#include <stdlib.h>

struct test_case {
    const char *name;
    int start_year, start_month, start_day, end_year, end_month, end_day;
    long long want;
};

static const struct test_case cases[] = {
    {"same-day", 2024, 3, 10, 2024, 3, 10, 0},
    {"next-day", 2024, 3, 10, 2024, 3, 11, 1},
    {"common-year", 2023, 1, 1, 2024, 1, 1, 365},
    {"leap-year", 2024, 1, 1, 2025, 1, 1, 366},
    {"end-before-start", 2024, 3, 11, 2024, 3, 10, -1},
    {"across-a-dst-change", 2024, 3, 9, 2024, 3, 11, 2},
    {"across-leap-day", 2024, 2, 28, 2024, 3, 1, 2},
    {"month-end-to-next-month", 2024, 1, 31, 2024, 3, 1, 30},
    {"year-1900-is-not-leap", 1900, 1, 1, 1901, 1, 1, 365},
    {"year-2000-is-leap", 2000, 1, 1, 2001, 1, 1, 366},
    {"400-years", 2000, 1, 1, 2400, 1, 1, 146097},
    {"across-the-epoch", 1969, 12, 31, 1970, 1, 1, 1},
    {"gregorian-reform-gap", 1582, 10, 4, 1582, 10, 15, 11},
    {"more-than-292-years", 1700, 1, 1, 2300, 1, 1, 219145},
    {"whole-range", 1, 1, 1, 9999, 12, 31, 3652058},
    {"whole-range-backward", 9999, 12, 31, 1, 1, 1, -3652058},
    {"year-99", 99, 12, 31, 100, 1, 1, 1},
};

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        long long got = days_between(c->start_year, c->start_month, c->start_day, c->end_year, c->end_month, c->end_day);
        if (got != c->want) {
            printf("FAIL %s: got %lld, want %lld\n", c->name, got, c->want);
            failures++;
        }
    }
    if (days_from_civil(1970, 1, 1) != 0 || days_from_civil(1, 1, 1) != -719162) {
        printf("FAIL days_from_civil\n");
        failures++;
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
