// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-between-time-zones/.

#define _POSIX_C_SOURCE 200809L

#include "tzconv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Days from 1970-01-01 to a date of the proleptic Gregorian calendar.
static long long days_from_civil(long long y, int m, int d) {
    y -= m <= 2;
    const long long era = (y >= 0 ? y : y - 399) / 400;
    const long long yoe = y - era * 400;
    const long long doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    const long long doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + doe - 719468;
}

static long long as_utc(const struct tm *tm) {
    return days_from_civil(tm->tm_year + 1900LL, tm->tm_mon + 1, tm->tm_mday) * 86400 +
           tm->tm_hour * 3600LL + tm->tm_min * 60LL + tm->tm_sec;
}

// localtime_r with an unknown TZ value uses UTC and reports no error, so
// the zone file is checked first.
static int zone_exists(const char *zone) {
    const char *dir = getenv("TZDIR");
    char path[512];
    if (strstr(zone, "..") != NULL || zone[0] == '/') {
        return 0;
    }
    snprintf(path, sizeof path, "%s/%s", dir != NULL ? dir : "/usr/share/zoneinfo", zone);
    return access(path, R_OK) == 0;
}

// The UTC offset, in seconds, of the zone in TZ at the instant t.
static long long offset_at(long long t) {
    const time_t tt = (time_t)t;
    struct tm tm;
    localtime_r(&tt, &tm);
    return as_utc(&tm) - t;
}

static void set_zone(const char *zone) {
    if (zone != NULL) {
        setenv("TZ", zone, 1);
    } else {
        unsetenv("TZ");
    }
    tzset();
}

int convert_time_zone(int year, int month, int day, int hour, int minute,
                      int second, const char *from_zone, const char *to_zone,
                      char out[TZCONV_SIZE]) {
    if (!zone_exists(from_zone) || !zone_exists(to_zone)) {
        return -1;
    }
    const char *old = getenv("TZ");
    char *saved = old != NULL ? strdup(old) : NULL;

    set_zone(from_zone);
    const long long t = days_from_civil(year, month, day) * 86400 + hour * 3600LL +
                        minute * 60LL + second;
    const long long offsets[2] = {offset_at(t - 86400), offset_at(t + 86400)};
    long long instant = t - offsets[0];  // a time in a gap
    int found = 0;
    for (int i = 0; i < 2; i++) {
        const long long u = t - offsets[i];
        if (offset_at(u) == offsets[i] && (!found || u < instant)) {
            instant = u;
            found = 1;
        }
    }

    set_zone(to_zone);
    const long long offset = offset_at(instant);
    const time_t tt = (time_t)instant;
    struct tm tm;
    localtime_r(&tt, &tm);

    set_zone(saved);
    free(saved);

    // The buffer has room for any int values, so the compiler can see that
    // snprintf does not truncate. For the years 1970 to 2100 the text has
    // TZCONV_SIZE - 1 characters.
    char text[128];
    const long long a = offset < 0 ? -offset : offset;
    const int n = snprintf(text, sizeof text, "%04d-%02d-%02dT%02d:%02d:%02d%c%02lld:%02lld",
                           tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
                           tm.tm_sec, offset < 0 ? '-' : '+', a / 3600, a % 3600 / 60);
    if (n != TZCONV_SIZE - 1) {
        return -1;
    }
    memcpy(out, text, TZCONV_SIZE);
    return 0;
}
