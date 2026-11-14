// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-days-to-a-date/.

#include "add.h"

#include <stdint.h>
#include <stdio.h>

static int is_leap(int64_t year) {
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

static int64_t days_in_month(int64_t year, int64_t month) {
    static const int64_t length[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return month == 2 && is_leap(year) ? 29 : length[month - 1];
}

// Division toward the past. The / of C goes toward zero.
static int64_t floor_div(int64_t a, int64_t b) {
    int64_t q = a / b;
    return (a % b != 0 && (a < 0) != (b < 0)) ? q - 1 : q;
}

// The days from 1970-01-01 to a date, and back (Howard Hinnant).
static int64_t days_from_civil(int64_t year, int64_t month, int64_t day) {
    int64_t y = month <= 2 ? year - 1 : year;
    int64_t era = floor_div(y, 400);
    int64_t yoe = y - era * 400;
    int64_t mp = (month + 9) % 12;
    int64_t doy = (153 * mp + 2) / 5 + day - 1;
    int64_t doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + doe - 719468;
}

static void civil_from_days(int64_t days, int64_t *year, int64_t *month, int64_t *day) {
    int64_t z = days + 719468;
    int64_t era = floor_div(z, 146097);
    int64_t doe = z - era * 146097;
    int64_t yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    int64_t doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    int64_t mp = (5 * doy + 2) / 153;
    *day = doy - (153 * mp + 2) / 5 + 1;
    *month = mp < 10 ? mp + 3 : mp - 9;
    *year = yoe + era * 400 + (*month <= 2);
}

int add_to_date(int year, int month, int day, int years, int months, int days, char *out, size_t size) {
    int64_t total = (int64_t)year * 12 + (month - 1) + (int64_t)years * 12 + months;
    int64_t new_year = floor_div(total, 12);
    int64_t new_month = total - new_year * 12 + 1;
    int64_t last = days_in_month(new_year, new_month);
    int64_t new_day = day < last ? day : last;
    int64_t y, m, d;
    civil_from_days(days_from_civil(new_year, new_month, new_day) + days, &y, &m, &d);
    int n = snprintf(out, size, "%04d-%02d-%02d", (int)y, (int)m, (int)d);
    return (n < 0 || (size_t)n >= size) ? -1 : 0;
}
