// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sleep-for-a-duration/.

#define _POSIX_C_SOURCE 200809L

#include "sleep.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct test_case {
    const char *name;
    long long duration_ns, start_ns;
    size_t n;
    int64_t readings[4];
    size_t want_n;
    int64_t want[4];
};

static const struct test_case cases[] = {
    {"one-call", 1000000LL, 0LL, 1, {1000000LL}, 1, {1000000LL}},
    {"woke-late", 1000000LL, 0LL, 1, {1080000LL}, 1, {1000000LL}},
    {"woke-early", 1000000LL, 0LL, 2, {600000LL, 1000000LL}, 2, {1000000LL, 400000LL}},
    {"one-second", 1000000000LL, 5000000000LL, 1, {6000050000LL}, 1, {1000000000LL}},
    {"zero", 0LL, 0LL, 0, {0}, 0, {0}},
    {"negative", -5000000LL, 0LL, 0, {0}, 0, {0}},
    {"most-negative", -4611686018427387904LL, 0LL, 0, {0}, 0, {0}},
    {"several-early-wakes", 10000000LL, 0LL, 4, {2000000LL, 5000000LL, 9999999LL, 10000000LL}, 4, {10000000LL, 8000000LL, 5000000LL, 1LL}},
    {"no-progress", 1000000LL, 0LL, 2, {0LL, 1000000LL}, 2, {1000000LL, 1000000LL}},
    {"exact-deadline", 3000000LL, 100LL, 1, {3000100LL}, 1, {3000000LL}},
    {"one-ns-short", 3000000LL, 0LL, 2, {2999999LL, 3000000LL}, 2, {3000000LL, 1LL}},
    {"extra-readings", 1000000LL, 0LL, 2, {1000000LL, 5000000LL}, 1, {1000000LL}},
    {"negative-origin", 2000000000LL, -5000000000LL, 2, {-4000000000LL, -3000000000LL}, 2, {2000000000LL, 1000000000LL}},
    {"origin-after-100-days", 1500000LL, 8640000000000000LL, 2, {8640000001000000LL, 8640000001500000LL}, 2, {1500000LL, 500000LL}},
    {"one-nanosecond", 1LL, 0LL, 1, {1000000LL}, 1, {1LL}},
    {"sub-millisecond", 1500000LL, 0LL, 2, {1000000LL, 1500000LL}, 2, {1500000LL, 500000LL}},
    {"one-hour", 3600000000000LL, 0LL, 1, {3600000000000LL}, 1, {3600000000000LL}},
    {"thirty-days", 2592000000000000LL, 0LL, 2, {2147483647000000LL, 2592000000000000LL}, 2, {2592000000000000LL, 444516353000000LL}},
};

static int64_t now_ns(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (int64_t)t.tv_sec * 1000000000 + t.tv_nsec;
}

int main(void) {
    int failed = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        int64_t got[8];
        const size_t n = sleep_requests(c->duration_ns, c->start_ns, c->readings, c->n, got, 8);
        if (n != c->want_n || memcmp(got, c->want, n * sizeof got[0]) != 0) {
            printf("FAIL %s: got %zu requests\n", c->name, n);
            failed = 1;
        }
    }
    int64_t start = now_ns();
    sleep_for_ns(20000000);
    if (now_ns() - start < 20000000) {
        printf("FAIL sleep_for_ns(20 ms) returned early\n");
        failed = 1;
    }
    const int64_t at_once[] = {0, -5000000};
    for (size_t i = 0; i < 2; i++) {
        start = now_ns();
        sleep_for_ns(at_once[i]);
        if (now_ns() - start >= 1000000000) {
            printf("FAIL sleep_for_ns(%lld) did not return at once\n", (long long)at_once[i]);
            failed = 1;
        }
    }
    if (failed) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
