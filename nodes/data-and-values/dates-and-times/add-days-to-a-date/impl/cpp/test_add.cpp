// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-days-to-a-date/.

#include <chrono>
#include <cstdio>
#include <string>

#include "add.hpp"

namespace {

struct test_case {
    const char* name;
    int year, month, day, years, months, days;
    const char* want;
};

const test_case cases[] = {
    {"one-day", 2024, 3, 10, 0, 0, 1, "2024-03-11"},
    {"one-month", 2024, 1, 15, 0, 1, 0, "2024-02-15"},
    {"one-year", 2023, 6, 1, 1, 0, 0, "2024-06-01"},
    {"day-into-next-year", 2023, 12, 31, 0, 0, 1, "2024-01-01"},
    {"nothing-to-add", 2024, 2, 29, 0, 0, 0, "2024-02-29"},
    {"month-end-to-leap-february", 2024, 1, 31, 0, 1, 0, "2024-02-29"},
    {"month-end-to-short-february", 2023, 1, 31, 0, 1, 0, "2023-02-28"},
    {"month-end-to-30-day-month", 2024, 3, 31, 0, 1, 0, "2024-04-30"},
    {"back-from-month-end", 2024, 5, 31, 0, -1, 0, "2024-04-30"},
    {"back-to-leap-february", 2024, 3, 31, 0, -1, 0, "2024-02-29"},
    {"leap-day-plus-one-year", 2024, 2, 29, 1, 0, 0, "2025-02-28"},
    {"leap-day-plus-four-years", 2024, 2, 29, 4, 0, 0, "2028-02-29"},
    {"year-and-month-at-once", 2024, 2, 29, 1, 1, 0, "2025-03-29"},
    {"months-before-days", 2024, 1, 31, 0, 1, 1, "2024-03-01"},
    {"month-into-next-year", 2023, 12, 15, 0, 1, 0, "2024-01-15"},
    {"thirteen-months-back", 2024, 1, 15, 0, -13, 0, "2022-12-15"},
    {"day-back-into-last-year", 2024, 1, 1, 0, 0, -1, "2023-12-31"},
    {"year-1900-is-not-leap", 1900, 2, 28, 0, 0, 1, "1900-03-01"},
    {"year-2100-is-not-leap", 2096, 2, 29, 4, 0, 0, "2100-02-28"},
    {"many-days", 1970, 1, 1, 0, 0, 2932896, "9999-12-31"},
    {"many-days-back", 9999, 12, 31, 0, 0, -3652058, "0001-01-01"},
    {"many-months", 1, 1, 31, 0, 119987, 0, "9999-12-31"},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        const std::string got = add_to_date(c.year, c.month, c.day, c.years, c.months, c.days);
        if (got != c.want) {
            std::printf("FAIL %s: got %s, want %s\n", c.name, got.c_str(), c.want);
            ++failures;
        }
    }
    using namespace std::chrono;
    const year_month_day not_ok = 2024y / January / 31d + months{1};
    if (not_ok.ok()) {
        std::printf("FAIL 2024-01-31 + 1 month should not be ok()\n");
        ++failures;
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
