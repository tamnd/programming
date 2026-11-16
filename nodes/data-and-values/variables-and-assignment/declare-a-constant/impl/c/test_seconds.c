// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-constant/.

#include "seconds.h"

#include <stdio.h>
#include <stdlib.h>

struct test_case {
    const char *name;
    long long days;
    long long want;
};

static const struct test_case cases[] = {
    {"one-day", 1, 86400},
    {"one-week", 7, 604800},
    {"one-year", 365, 31536000},
    {"zero", 0, 0},
    {"two-days", 2, 172800},
    {"leap-year", 366, 31622400},
    {"last-32-bit-day", 24855, 2147472000},
    {"past-32-bits", 24856, 2147558400LL},
    {"million-days", 1000000, 86400000000LL},
};

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        long long got = seconds_in(c->days);
        if (got != c->want) {
            printf("FAIL %s: got %lld\n", c->name, got);
            failures++;
        }
    }
    if (SECONDS_PER_DAY != 86400) {
        printf("FAIL SECONDS_PER_DAY\n");
        failures++;
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
