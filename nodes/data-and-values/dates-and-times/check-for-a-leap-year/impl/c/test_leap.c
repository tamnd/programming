// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-for-a-leap-year/.

#include "leap.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct test_case {
    const char *name;
    int year;
    bool want;
};

static const struct test_case cases[] = {
    {"divisible-by-4", 2024, true},
    {"not-divisible-by-4", 2023, false},
    {"century", 1900, false},
    {"divisible-by-400", 2000, true},
    {"next-century", 2100, false},
    {"year-1600", 1600, true},
    {"year-1700", 1700, false},
    {"year-2400", 2400, true},
    {"year-1", 1, false},
    {"year-4", 4, true},
    {"year-100", 100, false},
    {"year-1500-julian-leap", 1500, false},
    {"year-1582", 1582, false},
    {"year-9999", 9999, false},
    {"year-zero", 0, true},
    {"year-minus-1", -1, false},
    {"year-minus-4", -4, true},
    {"year-minus-100", -100, false},
    {"year-minus-400", -400, true},
};

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        if (is_leap_year(c->year) != c->want) {
            printf("FAIL %s\n", c->name);
            failures++;
        }
    }
    int count = 0;
    for (int year = 2000; year < 2400; year++) {
        count += is_leap_year(year);
    }
    if (count != 97) {
        printf("FAIL %d leap years in 400 years\n", count);
        failures++;
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
