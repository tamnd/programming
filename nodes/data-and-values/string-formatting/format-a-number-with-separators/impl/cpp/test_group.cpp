// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-separators/.

#include <climits>
#include <cstdint>
#include <cstdio>

#include "group.hpp"

namespace {

struct test_case {
    const char* name;
    std::int64_t n;
    const char* want;
};

const test_case cases[] = {
    {"million", 1234567, "1,234,567"},
    {"thousand", 1000, "1,000"},
    {"small", 42, "42"},
    {"zero", 0, "0"},
    {"three-digits", 999, "999"},
    {"negative", -1234567, "-1,234,567"},
    {"negative-small", -999, "-999"},
    {"negative-thousand", -1000, "-1,000"},
    {"int64-max", 9223372036854775807LL, "9,223,372,036,854,775,807"},
    {"int64-min", LLONG_MIN, "-9,223,372,036,854,775,808"},
    {"six-digits", 100000, "100,000"},
    {"seven-digits", 1000000, "1,000,000"},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        const std::string got = group_thousands(c.n);
        if (got != c.want) {
            std::printf("FAIL %s: got \"%s\"\n", c.name, got.c_str());
            ++failures;
        }
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
