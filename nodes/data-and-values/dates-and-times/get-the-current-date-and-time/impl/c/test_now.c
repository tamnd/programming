// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-current-date-and-time/.

#include "now.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct test_case {
    const char *name;
    int64_t epoch_ns;
    const char *want;
};

static const struct test_case cases[] = {
    {"epoch", INT64_C(0), "1970-01-01T00:00:00.000Z"},
    {"billennium", INT64_C(1000000000000000000), "2001-09-09T01:46:40.000Z"},
    {"with-milliseconds", INT64_C(1710054000123456789), "2024-03-10T07:00:00.123Z"},
    {"leap-day", INT64_C(1709208000000000000), "2024-02-29T12:00:00.000Z"},
    {"truncate-not-round", INT64_C(999999), "1970-01-01T00:00:00.000Z"},
    {"one-ns-before-epoch", INT64_C(-1), "1969-12-31T23:59:59.999Z"},
    {"one-ms-before-epoch", INT64_C(-1000000), "1969-12-31T23:59:59.999Z"},
    {"just-over-one-ms-before-epoch", INT64_C(-1000001), "1969-12-31T23:59:59.998Z"},
    {"last-ms-of-a-year", INT64_C(1704067199999999999), "2023-12-31T23:59:59.999Z"},
    {"year-2000-is-leap", INT64_C(951782400000000000), "2000-02-29T00:00:00.000Z"},
    {"year-2100-is-not-leap", INT64_C(4107542399999999999), "2100-02-28T23:59:59.999Z"},
    {"after-int32-seconds", INT64_C(2147483648000000000), "2038-01-19T03:14:08.000Z"},
    {"int64-max", INT64_C(9223372036854775807), "2262-04-11T23:47:16.854Z"},
    {"int64-min", INT64_MIN, "1677-09-21T00:12:43.145Z"},
};

// The fake clock returns one fixed reading and counts the calls.
static int64_t fake_now(void *ctx) {
    int *calls = ctx;
    ++*calls;
    return INT64_C(1710054000123456789);
}

int main(void) {
    int failed = 0;
    char buf[32];
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        int n = utc_timestamp(cases[i].epoch_ns, buf, sizeof buf);
        if (n != 24 || strcmp(buf, cases[i].want) != 0) {
            printf("FAIL %s: got %s (%d), want %s\n", cases[i].name, buf, n, cases[i].want);
            failed++;
        }
    }

    int calls = 0;
    now_utc(fake_now, &calls, buf, sizeof buf);
    if (strcmp(buf, "2024-03-10T07:00:00.123Z") != 0 || calls != 1) {
        printf("FAIL fake clock: got %s, %d calls\n", buf, calls);
        failed++;
    }

    int n = now_utc(system_ns, NULL, buf, sizeof buf);
    if (n != 24 || buf[10] != 'T' || buf[19] != '.' || buf[23] != 'Z' || atoi(buf) < 2024) {
        printf("FAIL real clock: got %s\n", buf);
        failed++;
    }

    if (failed == 0) {
        printf("ok\n");
    }
    return failed != 0;
}
