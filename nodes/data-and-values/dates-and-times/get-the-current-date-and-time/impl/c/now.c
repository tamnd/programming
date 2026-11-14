// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-current-date-and-time/.

#define _POSIX_C_SOURCE 200809L

#include "now.h"

#include <stdio.h>
#include <time.h>

int64_t system_ns(void *ctx) {
    (void)ctx;
    struct timespec ts;
    // TIME_UTC is the calendar time of the system clock. A person or NTP can set it.
    timespec_get(&ts, TIME_UTC);
    return (int64_t)ts.tv_sec * 1000000000 + ts.tv_nsec;
}

// floor_div divides and rounds toward the past. The / of C rounds toward zero.
static int64_t floor_div(int64_t a, int64_t b) {
    int64_t q = a / b;
    return (a % b < 0) ? q - 1 : q;
}

int utc_timestamp(int64_t epoch_ns, char *buf, size_t size) {
    int64_t ms = floor_div(epoch_ns, 1000000);
    int64_t secs = floor_div(ms, 1000);
    int millis = (int)(ms - secs * 1000);
    time_t t = (time_t)secs;
    struct tm tm;
    // gmtime_r fills the caller's struct. gmtime returns a static one that the next call overwrites.
    if (gmtime_r(&t, &tm) == NULL) {
        return -1;
    }
    char date[20];
    strftime(date, sizeof date, "%Y-%m-%dT%H:%M:%S", &tm);
    return snprintf(buf, size, "%s.%03dZ", date, millis);
}

int now_utc(now_clock *clock, void *clock_ctx, char *buf, size_t size) {
    return utc_timestamp(clock(clock_ctx), buf, size);
}
