// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-and-format-a-date/.

#include "to_utc.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// number reads len digits from s into *n. It returns false when one of them
// is not one of the ASCII digits 0 to 9.
static bool number(const char *s, int len, long long *n) {
    *n = 0;
    for (int i = 0; i < len; i++) {
        if (s[i] < '0' || s[i] > '9') {
            return false;
        }
        *n = *n * 10 + (s[i] - '0');
    }
    return true;
}

static long long days_in_month(long long year, long long month) {
    static const long long days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool leap = year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    return month == 2 && leap ? 29 : days[month - 1];
}

// floor_div is division that rounds down. The operator / rounds toward zero.
static long long floor_div(long long a, long long b) {
    return a / b - (a % b != 0 && (a < 0) != (b < 0));
}

// days_from_civil returns the days from 1970-01-01 to the date (Howard
// Hinnant). A year starts on 1 March here, so the leap day is the last day.
static long long days_from_civil(long long y, long long m, long long d) {
    y -= m <= 2;
    long long era = floor_div(y, 400);
    long long yoe = y - era * 400;
    long long doy = (153 * (m > 2 ? m - 3 : m + 9) + 2) / 5 + d - 1;
    long long doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + doe - 719468;
}

// civil_from_days is the inverse of days_from_civil.
static void civil_from_days(long long z, long long *y, long long *m, long long *d) {
    z += 719468;
    long long era = floor_div(z, 146097);
    long long doe = z - era * 146097;
    long long yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    long long doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    long long mp = (5 * doy + 2) / 153;
    *d = doy - (153 * mp + 2) / 5 + 1;
    *m = mp < 10 ? mp + 3 : mp - 9;
    *y = yoe + era * 400 + (*m <= 2);
}

int to_utc(const char *timestamp, char *out, size_t size) {
    const char *s = timestamp;
    size_t n = strlen(s);
    long long year, month, day, hour, minute, second;
    // YYYY-MM-DDTHH:MM:SS is 19 characters.
    if (n < 20 || !number(s, 4, &year) || s[4] != '-' || !number(s + 5, 2, &month) ||
        s[7] != '-' || !number(s + 8, 2, &day) || (s[10] != 'T' && s[10] != 't') ||
        !number(s + 11, 2, &hour) || s[13] != ':' || !number(s + 14, 2, &minute) ||
        s[16] != ':' || !number(s + 17, 2, &second)) {
        return -1;
    }
    if (month < 1 || month > 12 || day < 1 || day > days_in_month(year, month) || hour > 23 ||
        minute > 59 || second > 59) {
        return -1;
    }
    // The fraction: a dot and 1 to 3 digits, padded to milliseconds.
    size_t i = 19;
    long long ms = 0;
    if (s[i] == '.') {
        size_t start = ++i;
        while (i - start < 3 && s[i] >= '0' && s[i] <= '9') {
            ms = ms * 10 + (s[i++] - '0');
        }
        if (i == start) {
            return -1;
        }
        for (size_t k = i - start; k < 3; k++) {
            ms *= 10;
        }
    }
    // The offset: Z, or a sign, HH:MM. -00:00 is the same instant as Z.
    long long offset = 0;
    if ((s[i] == 'Z' || s[i] == 'z') && i + 1 == n) {
        offset = 0;
    } else if ((s[i] == '+' || s[i] == '-') && i + 6 == n && s[i + 3] == ':') {
        long long oh, om;
        if (!number(s + i + 1, 2, &oh) || !number(s + i + 4, 2, &om) || oh > 23 || om > 59) {
            return -1;
        }
        offset = (oh * 3600 + om * 60) * (s[i] == '+' ? 1 : -1);
    } else {
        return -1;
    }
    long long seconds =
        days_from_civil(year, month, day) * 86400 + hour * 3600 + minute * 60 + second - offset;
    long long total = seconds * 1000 + ms;
    long long days = floor_div(total, 86400000);
    long long rest = total - days * 86400000;
    long long y, m, d;
    civil_from_days(days, &y, &m, &d);
    // %04lld pads the year to 4 digits, as date-fullyear in RFC 3339 needs.
    snprintf(out, size, "%04lld-%02lld-%02lldT%02lld:%02lld:%02lld.%03lldZ", y, m, d,
             rest / 3600000, rest / 60000 % 60, rest / 1000 % 60, rest % 1000);
    return 0;
}
