// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-and-format-a-date/.

#ifndef TO_UTC_H
#define TO_UTC_H

#include <stddef.h>

// TO_UTC_SIZE is the size of the result: YYYY-MM-DDTHH:MM:SS.sssZ and a NUL.
#define TO_UTC_SIZE 25

// to_utc writes the instant of the RFC 3339 timestamp in UTC to out, as
// YYYY-MM-DDTHH:MM:SS.sssZ, and returns 0. It returns -1 and does not write
// to out when timestamp is not an RFC 3339 date-time with an offset, or when
// its date is not in the calendar. size is the size of out; it must be at
// least TO_UTC_SIZE.
int to_utc(const char *timestamp, char *out, size_t size);

#endif
