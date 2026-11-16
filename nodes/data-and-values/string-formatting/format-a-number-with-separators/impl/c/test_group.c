// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-separators/.

#include "group.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct test_case {
    const char *name;
    long long n;
    const char *want;
};

static const struct test_case cases[] = {
    {"million", 1234567, "1,234,567"},
    {"thousand", 1000, "1,000"},
    {"small", 42, "42"},
    {"zero", 0, "0"},
    {"three-digits", 999, "999"},
    {"negative", -1234567, "-1,234,567"},
    {"negative-small", -999, "-999"},
    {"negative-thousand", -1000, "-1,000"},
    {"int64-max", 9223372036854775807LL, "9,223,372,036,854,775,807"},
    {"int64-min", LLONG_MIN, "-9,223,372,036,854,775,808"},
    {"six-digits", 100000, "100,000"},
    {"seven-digits", 1000000, "1,000,000"},
};

int main(void) {
    int failures = 0;
    char buf[GROUP_THOUSANDS_SIZE];
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        group_thousands(buf, c->n);
        if (strcmp(buf, c->want) != 0) {
            printf("FAIL %s: got \"%s\"\n", c->name, buf);
            failures++;
        }
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
