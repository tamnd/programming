// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-fixed-decimals/.

#include "fixed.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct test_case {
    const char *name;
    double x;
    int digits;
    const char *want;
};

static const struct test_case cases[] = {
    {"pi-two", 3.14159, 2, "3.14"},
    {"pad-zeros", 1.0, 3, "1.000"},
    {"round-up", 1234.5678, 2, "1234.57"},
    {"zero-digits", 123.456, 0, "123"},
    {"float-error", 0.30000000000000004, 2, "0.30"},
    {"negative", -1.234, 1, "-1.2"},
    {"rounds-to-zero", 0.0049, 2, "0.00"},
    {"stored-below-2-675", 2.675, 2, "2.67"},
    {"stored-below-1-005", 1.005, 2, "1.00"},
    {"large", 1e+20, 2, "100000000000000000000.00"},
    {"many-digits", 0.1, 20, "0.10000000000000000555"},
    {"integer-value", 42.0, 2, "42.00"},
};

int main(void) {
    int failures = 0;
    char buf[64];
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        int n = fixed(buf, sizeof buf, c->x, c->digits);
        if (n != (int)strlen(c->want) || strcmp(buf, c->want) != 0) {
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
