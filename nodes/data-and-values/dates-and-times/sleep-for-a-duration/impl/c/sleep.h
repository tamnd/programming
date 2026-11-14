// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sleep-for-a-duration/.

#ifndef SLEEP_FOR_A_DURATION_H
#define SLEEP_FOR_A_DURATION_H

#include <stddef.h>
#include <stdint.h>

// A monotonic clock in nanoseconds, and a sleep for a number of nanoseconds.
// ctx is the state of a fake clock and a fake sleep, or NULL.
typedef int64_t (*sleep_clock_fn)(void *ctx);
typedef void (*sleep_fn)(void *ctx, int64_t ns);

// Sleeps until clock reaches the deadline. A sleep that ends early is
// followed by a sleep for the rest of the time.
void sleep_loop(int64_t duration_ns, sleep_clock_fn clock, sleep_fn sleep, void *ctx);

// Blocks the thread for at least duration_ns nanoseconds on CLOCK_MONOTONIC.
void sleep_for_ns(int64_t duration_ns);

// Runs sleep_loop on a fake clock that gives start_ns and then the n
// readings. Writes the durations that the loop asks for to out, at most cap
// of them, and returns their number.
size_t sleep_requests(int64_t duration_ns, int64_t start_ns, const int64_t *readings, size_t n,
                      int64_t *out, size_t cap);

#endif
