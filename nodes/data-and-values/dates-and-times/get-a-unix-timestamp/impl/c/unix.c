// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-unix-timestamp/.

#include "unix.h"

#include <time.h>

// Returns the number of days from 1970-01-01 to a date in the proleptic
// Gregorian calendar (the days_from_civil algorithm of Howard Hinnant).
static int64_t days_from_civil(int64_t year, int64_t month, int64_t day) {
    int64_t y = month <= 2 ? year - 1 : year;
    int64_t era = (y >= 0 ? y : y - 399) / 400;  // division toward the past
    int64_t yoe = y - era * 400;                  // 0 to 399
    int64_t mp = (month + 9) % 12;                // March is 0
    int64_t doy = (153 * mp + 2) / 5 + day - 1;   // 0 to 365
    int64_t doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + doe - 719468;
}

int64_t unix_timestamp(int year, int month, int day, int hour, int minute, int second, int offset_minutes) {
    // timegm and mktime need a time_t, and the range of time_t and of these
    // functions depends on the system. The arithmetic below has no such limit.
    return days_from_civil(year, month, day) * 86400 + (int64_t)hour * 3600 +
           ((int64_t)minute - offset_minutes) * 60 + second;
}

int64_t now_unix_seconds(void) {
    return (int64_t)time(NULL);
}

int64_t now_unix_millis(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    int64_t millis = (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    return millis;
}
