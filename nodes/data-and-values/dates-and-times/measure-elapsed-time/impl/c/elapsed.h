// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/measure-elapsed-time/.

#ifndef ELAPSED_H
#define ELAPSED_H

#include <stddef.h>
#include <stdint.h>

// A clock returns a reading in nanoseconds. ctx is passed through.
typedef int64_t elapsed_clock(void *ctx);

// The real clock: CLOCK_MONOTONIC in nanoseconds. ctx is not used.
int64_t monotonic_ns(void *ctx);

// Runs work(work_ctx) once and returns the time it took, in nanoseconds of clock.
int64_t measure(void (*work)(void *), void *work_ctx, elapsed_clock *clock, void *clock_ctx);

// Writes end_ns - start_ns as "M.UUU ms" to buf, with the rest under one microsecond dropped.
// Returns the length of the text, as snprintf does. 32 bytes are always enough.
int elapsed_ms(int64_t start_ns, int64_t end_ns, char *buf, size_t size);

#endif
