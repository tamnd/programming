// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/measure-elapsed-time/.

#define _POSIX_C_SOURCE 200809L

#include "elapsed.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int64_t monotonic_ns(void *ctx) {
    (void)ctx;
    struct timespec ts;
    // POSIX requires each implementation to support CLOCK_MONOTONIC, so this call does not fail.
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        abort();
    }
    // One int64_t holds the whole reading, so a subtraction needs no borrow from tv_sec.
    return (int64_t)ts.tv_sec * 1000000000 + ts.tv_nsec;
}

int64_t measure(void (*work)(void *), void *work_ctx, elapsed_clock *clock, void *clock_ctx) {
    int64_t start = clock(clock_ctx);
    work(work_ctx);
    return clock(clock_ctx) - start;
}

int elapsed_ms(int64_t start_ns, int64_t end_ns, char *buf, size_t size) {
    int64_t ns = end_ns - start_ns;
    // Integer division truncates, and ns is not negative.
    return snprintf(buf, size, "%" PRId64 ".%03" PRId64 " ms", ns / 1000000, ns / 1000 % 1000);
}
