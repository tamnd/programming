// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-difference-of-two-dates/.

#include "diff.h"

// The formula is from Howard Hinnant, "chrono-Compatible Low-Level Date
// Algorithms". It needs no time zone and no time_t, so it also works for
// years before 1970 and before 1900.
long long days_from_civil(long long year, int month, int day) {
    long long y = month <= 2 ? year - 1 : year;
    long long era = (y >= 0 ? y : y - 399) / 400;
    long long yoe = y - era * 400;
    long long mp = (month + 9) % 12;
    long long doy = (153 * mp + 2) / 5 + day - 1;
    long long doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + doe - 719468;
}

long long days_between(int start_year, int start_month, int start_day, int end_year, int end_month, int end_day) {
    return days_from_civil(end_year, end_month, end_day) - days_from_civil(start_year, start_month, start_day);
}
