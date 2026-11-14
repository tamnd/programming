// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-day-of-the-week/.

#include "weekday.h"

// The day arithmetic is from Howard Hinnant, "chrono-Compatible Low-Level
// Date Algorithms". It needs no time zone and no time_t, so it also works
// for years before 1970 and before 1900.
static long long days_from_civil(long long year, int month, int day) {
    long long y = month <= 2 ? year - 1 : year;
    long long era = (y >= 0 ? y : y - 399) / 400;
    long long yoe = y - era * 400;
    long long mp = (month + 9) % 12;
    long long doy = (153 * mp + 2) / 5 + day - 1;
    long long doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + doe - 719468;
}

static long long year_from_days(long long days) {
    long long z = days + 719468;
    long long era = (z >= 0 ? z : z - 146096) / 146097;
    long long doe = z - era * 146097;
    long long yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    long long doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    long long mp = (5 * doy + 2) / 153;
    return yoe + era * 400 + (mp >= 10 ? 1 : 0);
}

// 1970-01-01 was a Thursday, which is day 4. The % operator of C keeps the
// sign of the left operand, so a negative remainder needs 7 added.
static int weekday_from_days(long long days) {
    int r = (int)((days + 3) % 7);
    return (r < 0 ? r + 7 : r) + 1;
}

int day_of_week(int year, int month, int day) {
    return weekday_from_days(days_from_civil(year, month, day));
}

void iso_week(int year, int month, int day, int *week_year, int *week) {
    long long days = days_from_civil(year, month, day);
    long long thursday = days + 4 - weekday_from_days(days);
    long long y = year_from_days(thursday);
    *week_year = (int)y;
    *week = (int)((thursday - days_from_civil(y, 1, 1)) / 7 + 1);
}
