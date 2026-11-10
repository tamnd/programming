// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-and-format-a-date/.

#include "to_utc.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

// check compares to_utc(timestamp) with want. A NULL want means an error.
static void check(const char *name, const char *timestamp, const char *want) {
    char got[TO_UTC_SIZE] = "";
    int status = to_utc(timestamp, got, sizeof got);
    if (want == NULL && status != -1) {
        fprintf(stderr, "case %s: got \"%s\", want an error\n", name, got);
        failures++;
    } else if (want != NULL && (status != 0 || strcmp(got, want) != 0)) {
        fprintf(stderr, "case %s: got %d \"%s\", want \"%s\"\n", name, status, got, want);
        failures++;
    }
}

int main(void) {
    check("utc", "2026-11-10T12:00:00Z", "2026-11-10T12:00:00.000Z");
    check("positive-offset", "2026-11-10T19:30:00+07:00", "2026-11-10T12:30:00.000Z");
    check("negative-offset", "2026-11-10T07:00:00-05:00", "2026-11-10T12:00:00.000Z");
    check("back-a-day", "2026-11-11T01:00:00+07:00", "2026-11-10T18:00:00.000Z");
    check("forward-a-day", "2026-11-10T22:00:00-05:00", "2026-11-11T03:00:00.000Z");
    check("year-end", "2027-01-01T00:30:00+01:00", "2026-12-31T23:30:00.000Z");
    check("leap-day", "2024-02-29T23:00:00-02:00", "2024-03-01T01:00:00.000Z");
    check("half-hour-offset", "2026-11-10T17:30:00+05:30", "2026-11-10T12:00:00.000Z");
    check("quarter-hour-offset", "2026-11-10T17:45:00+05:45", "2026-11-10T12:00:00.000Z");
    check("unknown-offset", "2026-11-10T12:00:00-00:00", "2026-11-10T12:00:00.000Z");
    check("fraction-one-digit", "2026-11-10T12:00:00.5Z", "2026-11-10T12:00:00.500Z");
    check("fraction-three-digits", "2026-11-10T12:00:00.123+01:00", "2026-11-10T11:00:00.123Z");
    check("before-epoch", "1970-01-01T00:30:00+01:00", "1969-12-31T23:30:00.000Z");
    check("before-epoch-fraction", "1969-12-31T23:59:59.999Z", "1969-12-31T23:59:59.999Z");
    check("largest-offset", "2026-11-10T00:00:00+14:00", "2026-11-09T10:00:00.000Z");
    check("smallest-offset", "2026-11-10T00:00:00-12:00", "2026-11-10T12:00:00.000Z");
    check("lower-case", "2026-11-10t12:00:00z", "2026-11-10T12:00:00.000Z");
    check("year-one", "0001-01-01T00:00:00Z", "0001-01-01T00:00:00.000Z");
    check("year-9999", "9999-12-31T23:59:59.999Z", "9999-12-31T23:59:59.999Z");
    check("no-offset", "2026-11-10T12:00:00", NULL);
    check("date-only", "2026-11-10", NULL);
    check("space-separator", "2026-11-10 12:00:00Z", NULL);
    check("no-seconds", "2026-11-10T12:00Z", NULL);
    check("offset-no-colon", "2026-11-10T12:00:00+0700", NULL);
    check("offset-hours-only", "2026-11-10T12:00:00+07", NULL);
    check("not-a-leap-year", "2026-02-29T12:00:00Z", NULL);
    check("april-31", "2026-04-31T12:00:00Z", NULL);
    check("month-13", "2026-13-01T12:00:00Z", NULL);
    check("day-zero", "2026-11-00T12:00:00Z", NULL);
    check("hour-24", "2026-11-10T24:00:00Z", NULL);
    check("minute-60", "2026-11-10T12:60:00Z", NULL);
    check("leap-second", "2016-12-31T23:59:60Z", NULL);
    check("offset-hour-24", "2026-11-10T12:00:00+24:00", NULL);
    check("offset-minute-60", "2026-11-10T12:00:00+07:60", NULL);
    check("comma-fraction", "2026-11-10T12:00:00,5Z", NULL);
    check("trailing-space", "2026-11-10T12:00:00Z ", NULL);
    check("empty", "", NULL);
    if (failures > 0) {
        fprintf(stderr, "%d failures\n", failures);
        return 1;
    }
    return 0;
}
