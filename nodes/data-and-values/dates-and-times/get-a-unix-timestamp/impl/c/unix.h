// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-unix-timestamp/.

#ifndef UNIX_TIMESTAMP_H
#define UNIX_TIMESTAMP_H

#include <stdint.h>

// Returns the seconds since 1970-01-01T00:00:00Z of a local time with the
// given UTC offset in minutes.
int64_t unix_timestamp(int year, int month, int day, int hour, int minute, int second, int offset_minutes);

// Returns the current Unix time in whole seconds.
int64_t now_unix_seconds(void);

// Returns the current Unix time in whole milliseconds, rounded toward the past.
int64_t now_unix_millis(void);

#endif
