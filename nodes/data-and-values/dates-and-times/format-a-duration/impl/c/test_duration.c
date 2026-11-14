// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-duration/.

#include "duration.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct test_case {
    const char *name;
    long long seconds;
    const char *want;
};

static const struct test_case cases[] = {
    {"zero", 0LL, "00:00:00"},
    {"seconds-only", 59LL, "00:00:59"},
    {"one-minute", 60LL, "00:01:00"},
    {"one-hour", 3600LL, "01:00:00"},
    {"hours-minutes-seconds", 3723LL, "01:02:03"},
    {"just-under-an-hour", 3599LL, "00:59:59"},
    {"just-under-a-day", 86399LL, "23:59:59"},
    {"one-day", 86400LL, "24:00:00"},
    {"more-than-a-day", 90061LL, "25:01:01"},
    {"hundred-hours", 360000LL, "100:00:00"},
    {"one-week", 604800LL, "168:00:00"},
    {"one-year", 31536000LL, "8760:00:00"},
    {"minus-one-second", -1LL, "-00:00:01"},
    {"negative-minute", -61LL, "-00:01:01"},
    {"negative-hours", -3723LL, "-01:02:03"},
    {"negative-day", -90061LL, "-25:01:01"},
    {"beyond-go-duration", 10000000000LL, "2777777:46:40"},
    {"largest", 1000000000000000LL, "277777777777:46:40"},
    {"most-negative", -1000000000000000LL, "-277777777777:46:40"},
};

int main(void) {
    int failures = 0;
    char out[FORMAT_DURATION_SIZE];
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        format_duration(c->seconds, out);
        if (strcmp(out, c->want) != 0) {
            printf("FAIL %s: %s\n", c->name, out);
            failures++;
        }
    }
    format_duration(LLONG_MIN, out);
    if (strcmp(out, "-2562047788015215:30:08") != 0) {
        printf("FAIL LLONG_MIN: %s\n", out);
        failures++;
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
