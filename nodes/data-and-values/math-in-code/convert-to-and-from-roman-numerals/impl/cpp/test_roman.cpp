// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-to-and-from-roman-numerals/.

#include <cstdio>
#include <stdexcept>
#include <string_view>

#include "roman.hpp"

namespace {

struct test_case {
    const char* name;
    int n;
    const char* want;
};

const test_case cases[] = {
    {"year-1990", 1990, "MCMXC"},
    {"year-2008", 2008, "MMVIII"},
    {"each-symbol-once", 1666, "MDCLXVI"},
    {"one", 1, "I"},
    {"four", 4, "IV"},
    {"nine", 9, "IX"},
    {"forty-nine", 49, "XLIX"},
    {"ninety-nine", 99, "XCIX"},
    {"four-hundred-forty-four", 444, "CDXLIV"},
    {"year-1994", 1994, "MCMXCIV"},
    {"longest", 3888, "MMMDCCCLXXXVIII"},
    {"largest", 3999, "MMMCMXCIX"},
};

const std::string_view invalid[] = {"", "IIII", "IC", "VX", "MMMM", "iv", "ABC", "IIV", "XM"};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        if (to_roman(c.n) != c.want || from_roman(c.want) != c.n) {
            std::printf("FAIL %s\n", c.name);
            ++failures;
        }
    }
    for (int n = 1; n <= 3999; ++n) {
        if (from_roman(to_roman(n)) != n) {
            std::printf("FAIL round trip %d\n", n);
            ++failures;
        }
    }
    for (const auto s : invalid) {
        if (from_roman(s).has_value()) {
            std::printf("FAIL invalid\n");
            ++failures;
        }
    }
    for (const int n : {0, -1, 4000}) {
        try {
            (void)to_roman(n);
            std::printf("FAIL out of range %d\n", n);
            ++failures;
        } catch (const std::out_of_range&) {
        }
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
