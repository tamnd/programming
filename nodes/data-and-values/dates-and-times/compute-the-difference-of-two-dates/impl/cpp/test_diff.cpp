// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-difference-of-two-dates/.

#include <chrono>
#include <cstdio>

#include "diff.hpp"

namespace {

struct test_case {
    const char* name;
    int start_year, start_month, start_day, end_year, end_month, end_day;
    long long want;
};

const test_case cases[] = {
    {"same-day", 2024, 3, 10, 2024, 3, 10, 0},
    {"next-day", 2024, 3, 10, 2024, 3, 11, 1},
    {"common-year", 2023, 1, 1, 2024, 1, 1, 365},
    {"leap-year", 2024, 1, 1, 2025, 1, 1, 366},
    {"end-before-start", 2024, 3, 11, 2024, 3, 10, -1},
    {"across-a-dst-change", 2024, 3, 9, 2024, 3, 11, 2},
    {"across-leap-day", 2024, 2, 28, 2024, 3, 1, 2},
    {"month-end-to-next-month", 2024, 1, 31, 2024, 3, 1, 30},
    {"year-1900-is-not-leap", 1900, 1, 1, 1901, 1, 1, 365},
    {"year-2000-is-leap", 2000, 1, 1, 2001, 1, 1, 366},
    {"400-years", 2000, 1, 1, 2400, 1, 1, 146097},
    {"across-the-epoch", 1969, 12, 31, 1970, 1, 1, 1},
    {"gregorian-reform-gap", 1582, 10, 4, 1582, 10, 15, 11},
    {"more-than-292-years", 1700, 1, 1, 2300, 1, 1, 219145},
    {"whole-range", 1, 1, 1, 9999, 12, 31, 3652058},
    {"whole-range-backward", 9999, 12, 31, 1, 1, 1, -3652058},
    {"year-99", 99, 12, 31, 100, 1, 1, 1},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        const long long got = days_between(c.start_year, c.start_month, c.start_day, c.end_year, c.end_month, c.end_day);
        if (got != c.want) {
            std::printf("FAIL %s: got %lld, want %lld\n", c.name, got, c.want);
            ++failures;
        }
    }
    using namespace std::chrono;
    const auto months_apart = (2024y / March) - (2024y / January);
    if (months_apart.count() != 2) {
        std::printf("FAIL year_month difference\n");
        ++failures;
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
