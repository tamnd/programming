// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-big-integers/.

#include "big_power.hpp"

#include <cstdint>
#include <cstdio>
#include <limits>
#include <string>

namespace {

struct TestCase {
    const char *name;
    int base, exp;
    std::string want;
};

const TestCase cases[] = {
    {"small", 2, 10, "1024"},
    {"two-to-the-100", 2, 100, "1267650600228229401496703205376"},
    {"zero-exponent", 7, 0, "1"},
    {"zero-to-the-zero", 0, 0, "1"},
    {"zero-base", 0, 5, "0"},
    {"minus-one-odd", -1, 199, "-1"},
    {"negative-odd", -3, 41, "-36472996377170786403"},
    {"negative-even", -3, 40, "12157665459056928801"},
    {"int64-max-plus-one", 2, 63, "9223372036854775808"},
    {"uint64-max-plus-one", 2, 64, "18446744073709551616"},
    {"two-to-the-200", 2, 200, "1606938044258990275541962092341162602522202993782792835301376"},
    {"ten-to-the-30", 10, 30, "1" + std::string(30, '0')},
    {"largest", 1000, 200, "1" + std::string(600, '0')},
    {"smallest", -1000, 199, "-1" + std::string(597, '0')},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto &c : cases) {
        if (big_power(c.base, c.exp) != c.want) {
            std::fprintf(stderr, "case %s failed\n", c.name);
            failures++;
        }
    }
    // 2^63 fits in a uint64_t but not in an int64_t.
    const std::uint64_t big = static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max()) + 1;
    if (std::to_string(big) != big_power(2, 63)) {
        std::fprintf(stderr, "uint64 check failed\n");
        failures++;
    }
    if (failures != 0) {
        return 1;
    }
    std::puts("ok");
    return 0;
}
