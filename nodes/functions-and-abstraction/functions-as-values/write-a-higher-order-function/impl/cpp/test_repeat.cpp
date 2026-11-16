// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-higher-order-function/.

#include <cstdio>

#include "repeat.hpp"

namespace {

struct test_case {
    const char* name;
    int start, times, want;
};

const test_case cases[] = {
    {"one-step", 6, 1, 3},
    {"three-steps", 6, 3, 5},
    {"ten-steps", 27, 10, 214},
    {"zero-times", 7, 0, 7},
    {"start-one", 1, 1, 4},
    {"cycle", 1, 3, 1},
    {"past-one", 16, 10, 1},
    {"peak-27", 27, 77, 9232},
    {"peak-9663", 9663, 48, 27114424},
    {"limit", 10000, 1000, 2},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        const int got = higher_order::collatz_after(c.start, c.times);
        if (got != c.want) {
            std::printf("FAIL %s: got %d, want %d\n", c.name, got, c.want);
            ++failures;
        }
    }
    const int doubled = higher_order::repeat([](int x) { return x * 2; }, 10)(1);
    if (doubled != 1024) {
        std::printf("FAIL repeat with a lambda: got %d, want 1024\n", doubled);
        ++failures;
    }
    const int same = higher_order::repeat(higher_order::collatz_step, 0)(27);
    if (same != 27) {
        std::printf("FAIL repeat zero times: got %d, want 27\n", same);
        ++failures;
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
