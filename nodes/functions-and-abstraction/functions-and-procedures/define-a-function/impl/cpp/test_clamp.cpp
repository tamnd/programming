// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-function/.

#include <climits>
#include <cstdio>

#include "clamp.hpp"

namespace {

struct test_case {
    const char* name;
    int value, low, high, want;
};

const test_case cases[] = {
    {"inside", 5, 1, 10, 5},
    {"below", -3, 0, 10, 0},
    {"above", 42, 0, 10, 10},
    {"at-low", 0, 0, 10, 0},
    {"at-high", 10, 0, 10, 10},
    {"equal-bounds", 7, 3, 3, 3},
    {"negative-range", -50, -20, -10, -20},
    {"zero", 0, -5, 5, 0},
    {"int-max", INT_MAX, -100, 100, 100},
    {"int-min", INT_MIN, -100, 100, -100},
    {"full-range", INT_MIN, INT_MIN, INT_MAX, INT_MIN},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        const int got = limits::clamp(c.value, c.low, c.high);
        if (got != c.want) {
            std::printf("FAIL %s: got %d, want %d\n", c.name, got, c.want);
            ++failures;
        }
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
