// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/measure-elapsed-time/.

#include "elapsed.h"

#include <stdio.h>
#include <string.h>

struct test_case {
    const char *name;
    int64_t start_ns, end_ns;
    const char *want;
};

static const struct test_case cases[] = {
    {"one-and-a-half-ms", INT64_C(1000000000), INT64_C(1001500000), "1.500 ms"},
    {"seconds", INT64_C(0), INT64_C(2345678901), "2345.678 ms"},
    {"under-one-ms", INT64_C(50000), INT64_C(750000), "0.700 ms"},
    {"zero", INT64_C(123456789), INT64_C(123456789), "0.000 ms"},
    {"under-one-microsecond", INT64_C(0), INT64_C(999), "0.000 ms"},
    {"truncate-not-round", INT64_C(0), INT64_C(1999999), "1.999 ms"},
    {"leading-zero-in-fraction", INT64_C(0), INT64_C(1005000), "1.005 ms"},
    {"exact-ms", INT64_C(0), INT64_C(7000000), "7.000 ms"},
    {"one-hour", INT64_C(0), INT64_C(3600000000000), "3600000.000 ms"},
    {"negative-readings", INT64_C(-5000000), INT64_C(-3500000), "1.500 ms"},
    {"across-zero", INT64_C(-1000000), INT64_C(500000), "1.500 ms"},
    {"beyond-2-pow-53", INT64_C(9007199254740993), INT64_C(9007199255741992), "1.000 ms"},
    {"near-int64-max", INT64_C(9223372035854775807), INT64_C(9223372036854775807), "1000.000 ms"},
    {"near-int64-min", INT64_MIN, INT64_C(-9223372035854775808), "1000.000 ms"},
};

// The fake clock returns its readings in order and logs each call.
struct fake_clock {
    const int64_t *readings;
    size_t next;
    char *log;
};

static int64_t fake_now(void *ctx) {
    struct fake_clock *c = ctx;
    strcat(c->log, "clock ");
    return c->readings[c->next++];
}

static void log_work(void *ctx) {
    strcat(ctx, "work ");
}

int main(void) {
    int failed = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        char got[32];
        elapsed_ms(c->start_ns, c->end_ns, got, sizeof got);
        if (strcmp(got, c->want) != 0) {
            fprintf(stderr, "FAIL %s: got \"%s\"\n", c->name, got);
            failed++;
        }
    }

    char log[64] = "";
    const int64_t readings[] = {5000000, 6500000};
    struct fake_clock fake = {readings, 0, log};
    int64_t got = measure(log_work, log, fake_now, &fake);
    if (got != 1500000 || strcmp(log, "clock work clock ") != 0) {
        fprintf(stderr, "FAIL fake clock: got %lld, log \"%s\"\n", (long long)got, log);
        failed++;
    }

    char real_log[64] = "";
    if (measure(log_work, real_log, monotonic_ns, nullptr) < 0 || strcmp(real_log, "work ") != 0) {
        fprintf(stderr, "FAIL monotonic clock\n");
        failed++;
    }

    if (failed > 0) {
        return 1;
    }
    printf("ok\n");
    return 0;
}
