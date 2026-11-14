// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-current-date-and-time/.

#ifndef NOW_H
#define NOW_H

#include <stddef.h>
#include <stdint.h>

// A clock returns nanoseconds since 1970-01-01T00:00:00Z. ctx is passed through.
typedef int64_t now_clock(void *ctx);

// The real clock: timespec_get with TIME_UTC, in nanoseconds. ctx is not used.
int64_t system_ns(void *ctx);

// Writes the UTC date and time of epoch_ns as "YYYY-MM-DDTHH:MM:SS.mmmZ" to buf.
// Returns the length of the text, as snprintf does, or -1 when the date has no
// broken-down form. 25 bytes are always enough.
int utc_timestamp(int64_t epoch_ns, char *buf, size_t size);

// Reads clock once and writes the UTC date and time of the reading to buf.
int now_utc(now_clock *clock, void *clock_ctx, char *buf, size_t size);

#endif
