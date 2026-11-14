// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-unix-timestamp/.

#define _DEFAULT_SOURCE

#include "unix.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct test_case {
    const char *name;
    int year, month, day, hour, minute, second, offset;
    long long want;
};

static const struct test_case cases[] = {
    {"epoch", 1970, 1, 1, 0, 0, 0, 0, 0LL},
    {"billennium", 2001, 9, 9, 1, 46, 40, 0, 1000000000LL},
    {"negative-offset", 2024, 3, 10, 2, 0, 0, -300, 1710054000LL},
    {"positive-offset", 2024, 3, 10, 16, 0, 0, 540, 1710054000LL},
    {"one-second-before-epoch", 1969, 12, 31, 23, 59, 59, 0, -1LL},
    {"offset-moves-before-epoch", 1970, 1, 1, 0, 0, 0, 60, -3600LL},
    {"offset-moves-to-next-year", 2023, 12, 31, 19, 0, 0, -300, 1704067200LL},
    {"offset-of-45-minutes", 2024, 1, 1, 5, 45, 0, 345, 1704067200LL},
    {"offset-of-minus-9-30", 2023, 12, 31, 14, 30, 0, -570, 1704067200LL},
    {"offset-plus-14", 1970, 1, 1, 14, 0, 0, 840, 0LL},
    {"offset-minus-12", 1969, 12, 31, 12, 0, 0, -720, 0LL},
    {"leap-day", 2024, 2, 29, 0, 0, 0, 0, 1709164800LL},
    {"year-2000-is-leap", 2000, 2, 29, 0, 0, 0, 0, 951782400LL},
    {"year-1900-is-not-leap", 1900, 3, 1, 0, 0, 0, 0, -2203891200LL},
    {"int32-max", 2038, 1, 19, 3, 14, 7, 0, 2147483647LL},
    {"after-int32-max", 2038, 1, 19, 3, 14, 8, 0, 2147483648LL},
    {"int32-min", 1901, 12, 13, 20, 45, 52, 0, -2147483648LL},
    {"year-99-is-not-1999", 99, 12, 31, 23, 59, 59, 0, -59011459201LL},
    {"first-second-of-year-1", 1, 1, 1, 0, 0, 0, 0, -62135596800LL},
    {"last-second-of-year-9999", 9999, 12, 31, 23, 59, 59, 0, 253402300799LL},
};

static int failures = 0;

static void expect(int ok, const char *what, const char *name) {
    if (!ok) {
        printf("FAIL %s: %s\n", name, what);
        failures++;
    }
}

int main(void) {
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        int64_t got = unix_timestamp(c->year, c->month, c->day, c->hour, c->minute, c->second, c->offset);
        expect(got == c->want, "unix_timestamp", c->name);

        time_t back = (time_t)(c->want + (int64_t)c->offset * 60);
        struct tm tm;
        char text[96], want_text[96];
        if (gmtime_r(&back, &tm) == NULL) {
            expect(0, "gmtime_r", c->name);
            continue;
        }
        snprintf(text, sizeof text, "%04d-%02d-%02dT%02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                 tm.tm_hour, tm.tm_min, tm.tm_sec);
        snprintf(want_text, sizeof want_text, "%04d-%02d-%02dT%02d:%02d:%02d", c->year, c->month, c->day, c->hour,
                 c->minute, c->second);
        expect(strcmp(text, want_text) == 0, "back to a date", c->name);
    }

    int64_t seconds = now_unix_seconds();
    int64_t millis = now_unix_millis();
    expect(seconds >= 1704067200, "now_unix_seconds is in 2024 or later", "now");
    int64_t diff = millis / 1000 - seconds;
    expect(diff >= 0 && diff <= 1, "seconds and milliseconds agree", "now");

    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
