// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-big-integers/.

#include "big_power.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct test_case {
    const char *name;
    int base, exp;
    const char *want;
};

static const struct test_case cases[] = {
    {"small", 2, 10, "1024"},
    {"two-to-the-100", 2, 100, "1267650600228229401496703205376"},
    {"zero-exponent", 7, 0, "1"},
    {"zero-to-the-zero", 0, 0, "1"},
    {"zero-base", 0, 5, "0"},
    {"minus-one-odd", -1, 199, "-1"},
    {"negative-odd", -3, 41, "-36472996377170786403"},
    {"negative-even", -3, 40, "12157665459056928801"},
    {"int64-max-plus-one", 2, 63, "9223372036854775808"},
    {"uint64-max-plus-one", 2, 64, "18446744073709551616"},
    {"two-to-the-200", 2, 200, "1606938044258990275541962092341162602522202993782792835301376"},
};

static int check(const char *name, int base, int exp, const char *want) {
    char *got = big_power(base, exp);
    int ok = got != NULL && strcmp(got, want) == 0;
    if (!ok) {
        fprintf(stderr, "case %s failed\n", name);
    }
    free(got);
    return ok ? 0 : 1;
}

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        failures += check(cases[i].name, cases[i].base, cases[i].exp, cases[i].want);
    }
    // Long results: a one, or a minus sign and a one, then zeros.
    char want[603];
    want[0] = '1';
    memset(want + 1, '0', 30);
    want[31] = '\0';
    failures += check("ten-to-the-30", 10, 30, want);
    memset(want + 1, '0', 600);
    want[601] = '\0';
    failures += check("largest", 1000, 200, want);
    want[0] = '-';
    want[1] = '1';
    memset(want + 2, '0', 597);
    want[599] = '\0';
    failures += check("smallest", -1000, 199, want);
    // 2^63 fits in a uint64_t but not in an int64_t; 2^64 fits in neither.
    char buf[32];
    snprintf(buf, sizeof buf, "%" PRIu64, (uint64_t)INT64_MAX + 1);
    failures += check("int64-max-plus-one-uint64", 2, 63, buf);
    if (UINT64_MAX + (uint64_t)1 != 0) {
        fprintf(stderr, "uint64 wrap failed\n");
        failures++;
    }
    if (failures != 0) {
        return 1;
    }
    puts("ok");
    return 0;
}
