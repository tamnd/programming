// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sleep-for-a-duration/.

#define _POSIX_C_SOURCE 200809L

#include "sleep.h"

#include <time.h>

void sleep_loop(int64_t duration_ns, sleep_clock_fn clock, sleep_fn sleep, void *ctx) {
    int64_t now = clock(ctx);
    const int64_t deadline = now + duration_ns;
    while (now < deadline) {
        sleep(ctx, deadline - now);
        now = clock(ctx);
    }
}

static int64_t monotonic_ns(void *ctx) {
    (void)ctx;
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (int64_t)t.tv_sec * 1000000000 + t.tv_nsec;
}

// One call of nanosleep. A signal can end it early with EINTR. The loop then
// reads the clock and sleeps for the rest, so the error needs no handling.
static void nanosleep_ns(void *ctx, int64_t ns) {
    (void)ctx;
    const struct timespec d = {.tv_sec = (time_t)(ns / 1000000000), .tv_nsec = (long)(ns % 1000000000)};
    (void)nanosleep(&d, NULL);
}

void sleep_for_ns(int64_t duration_ns) {
    sleep_loop(duration_ns, monotonic_ns, nanosleep_ns, NULL);
}

struct fake {
    int64_t start_ns;
    const int64_t *readings;
    size_t n, next;
    int64_t *out;
    size_t cap, count;
};

static int64_t fake_clock(void *ctx) {
    struct fake *f = ctx;
    if (f->next == 0) {
        f->next = 1;
        return f->start_ns;
    }
    // The contract gives enough readings. If they run out, the clock jumps to
    // the end of time, so that the loop stops.
    const size_t i = f->next - 1;
    f->next++;
    return i < f->n ? f->readings[i] : INT64_MAX;
}

static void fake_sleep(void *ctx, int64_t ns) {
    struct fake *f = ctx;
    if (f->count < f->cap) {
        f->out[f->count] = ns;
    }
    f->count++;
}

size_t sleep_requests(int64_t duration_ns, int64_t start_ns, const int64_t *readings, size_t n,
                      int64_t *out, size_t cap) {
    struct fake f = {start_ns, readings, n, 0, out, cap, 0};
    sleep_loop(duration_ns, fake_clock, fake_sleep, &f);
    return f.count < cap ? f.count : cap;
}
