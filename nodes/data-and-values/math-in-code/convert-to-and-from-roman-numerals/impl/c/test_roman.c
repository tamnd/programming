// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-to-and-from-roman-numerals/.

#include "roman.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct test_case {
    const char *name;
    int n;
    const char *want;
};

static const struct test_case cases[] = {
    {"year-1990", 1990, "MCMXC"},
    {"year-2008", 2008, "MMVIII"},
    {"each-symbol-once", 1666, "MDCLXVI"},
    {"one", 1, "I"},
    {"four", 4, "IV"},
    {"nine", 9, "IX"},
    {"forty-nine", 49, "XLIX"},
    {"ninety-nine", 99, "XCIX"},
    {"four-hundred-forty-four", 444, "CDXLIV"},
    {"year-1994", 1994, "MCMXCIV"},
    {"longest", 3888, "MMMDCCCLXXXVIII"},
    {"largest", 3999, "MMMCMXCIX"},
};

static const char *const invalid[] = {"", "IIII", "IC", "VX", "MMMM", "iv", "ABC", "IIV", "XM"};

int main(void) {
    int failures = 0;
    char buf[ROMAN_SIZE];
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        if (to_roman(c->n, buf, sizeof buf) != 0 || strcmp(buf, c->want) != 0) {
            printf("FAIL %s: to_roman\n", c->name);
            failures++;
        }
        if (from_roman(c->want) != c->n) {
            printf("FAIL %s: from_roman\n", c->name);
            failures++;
        }
    }
    for (int n = 1; n <= 3999; n++) {
        if (to_roman(n, buf, sizeof buf) != 0 || from_roman(buf) != n) {
            printf("FAIL round trip %d\n", n);
            failures++;
        }
    }
    for (size_t i = 0; i < sizeof invalid / sizeof invalid[0]; i++) {
        if (from_roman(invalid[i]) != -1) {
            printf("FAIL invalid \"%s\"\n", invalid[i]);
            failures++;
        }
    }
    if (to_roman(0, buf, sizeof buf) != -1 || to_roman(4000, buf, sizeof buf) != -1) {
        printf("FAIL out of range\n");
        failures++;
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
