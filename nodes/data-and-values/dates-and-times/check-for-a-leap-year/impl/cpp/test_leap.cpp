// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-for-a-leap-year/.

#include <cstdio>

#include "leap.hpp"

namespace {

struct test_case {
    const char* name;
    int year;
    bool want;
};

const test_case cases[] = {
    {"divisible-by-4", 2024, true},
    {"not-divisible-by-4", 2023, false},
    {"century", 1900, false},
    {"divisible-by-400", 2000, true},
    {"next-century", 2100, false},
    {"year-1600", 1600, true},
    {"year-1700", 1700, false},
    {"year-2400", 2400, true},
    {"year-1", 1, false},
    {"year-4", 4, true},
    {"year-100", 100, false},
    {"year-1500-julian-leap", 1500, false},
    {"year-1582", 1582, false},
    {"year-9999", 9999, false},
    {"year-zero", 0, true},
    {"year-minus-1", -1, false},
    {"year-minus-4", -4, true},
    {"year-minus-100", -100, false},
    {"year-minus-400", -400, true},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        if (is_leap_year(c.year) != c.want) {
            std::printf("FAIL %s\n", c.name);
            ++failures;
        }
    }
    for (int year = -9999; year <= 9999; ++year) {
        const bool rule = year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
        if (is_leap_year(year) != rule) {
            std::printf("FAIL %d\n", year);
            ++failures;
        }
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
