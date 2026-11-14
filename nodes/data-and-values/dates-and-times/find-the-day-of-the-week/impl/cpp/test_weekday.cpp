// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-day-of-the-week/.

#include <cstdio>
#include <utility>

#include "weekday.hpp"

namespace {

struct test_case {
    const char* name;
    int year, month, day;
    int want;
};

const test_case cases[] = {
    {"a-sunday", 2024, 3, 10, 7},
    {"a-monday", 2024, 3, 11, 1},
    {"the-epoch", 1970, 1, 1, 4},
    {"year-2000", 2000, 1, 1, 6},
    {"a-saturday", 2024, 3, 16, 6},
    {"leap-day", 2024, 2, 29, 4},
    {"after-leap-day-2000", 2000, 3, 1, 3},
    {"year-1900-is-not-leap", 1900, 3, 1, 4},
    {"year-2100-is-not-leap", 2100, 3, 1, 1},
    {"before-the-epoch", 1969, 12, 31, 3},
    {"gregorian-reform", 1582, 10, 15, 5},
    {"day-before-the-reform", 1582, 10, 4, 1},
    {"first-day", 1, 1, 1, 1},
    {"last-day", 9999, 12, 31, 5},
    {"year-99", 99, 12, 31, 4},
    {"iso-week-of-next-year", 2024, 12, 30, 1},
    {"iso-week-of-last-year", 2027, 1, 1, 5},
    {"month-end", 2024, 4, 30, 2},
};

struct week_case {
    int year, month, day;
    int week_year, week;
};

const week_case weeks[] = {
    {2024, 3, 10, 2024, 10},
    {2024, 12, 30, 2025, 1},
    {2027, 1, 1, 2026, 53},
    {2000, 1, 1, 1999, 52},
    {1969, 12, 31, 1970, 1},
    {99, 12, 31, 99, 53},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        const int got = day_of_week(c.year, c.month, c.day);
        if (got != c.want) {
            std::printf("FAIL %s: got %d, want %d\n", c.name, got, c.want);
            ++failures;
        }
    }
    for (const auto& c : weeks) {
        const auto got = iso_week(c.year, c.month, c.day);
        if (got != std::pair{c.week_year, c.week}) {
            std::printf("FAIL iso week of %04d-%02d-%02d: got %d-W%02d\n", c.year, c.month, c.day, got.first, got.second);
            ++failures;
        }
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
