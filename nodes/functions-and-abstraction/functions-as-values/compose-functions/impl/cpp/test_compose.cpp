// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compose-functions/.

#include <cstdio>
#include <string>
#include <vector>

#include "compose.hpp"

namespace {

std::vector<std::string> times(const std::vector<std::string>& names, int n) {
    std::vector<std::string> out;
    for (int i = 0; i < n; ++i) {
        out.insert(out.end(), names.begin(), names.end());
    }
    return out;
}

struct test_case {
    const char* name;
    std::vector<std::string> ops;
    int x, want;
};

}  // namespace

int main() {
    const std::vector<test_case> cases = {
        {"inc-after-double", {"inc", "double"}, 3, 7},
        {"double-after-inc", {"double", "inc"}, 3, 8},
        {"three-functions", {"negate", "dec", "double"}, 5, -9},
        {"empty", {}, 42, 42},
        {"single-zero", {"negate"}, 0, 0},
        {"negative", {"dec", "negate"}, -5, 4},
        {"low", {"inc"}, -1000, -999},
        {"duplicates", {"inc", "inc", "inc"}, 7, 10},
        {"limit-high", times({"double"}, 20), 1000, 1048576000},
        {"limit-low", times({"double"}, 20), -1000, -1048576000},
        {"mixed-20", times({"double", "inc"}, 10), 1000, 1026046},
        {"mixed-20-swapped", times({"inc", "double"}, 10), 1000, 1025023},
    };
    int failures = 0;
    for (const auto& c : cases) {
        const int got = composition::compose_all(c.ops, c.x);
        if (got != c.want) {
            std::printf("FAIL %s: got %d, want %d\n", c.name, got, c.want);
            ++failures;
        }
    }
    const auto shout = composition::compose<std::string>({
        [](std::string s) { return s + "!"; },
        [](std::string s) { return s + s; },
    });
    if (shout("ab") != "abab!") {
        std::printf("FAIL compose with strings\n");
        ++failures;
    }
    if (composition::compose<int>({})(5) != 5) {
        std::printf("FAIL compose of nothing\n");
        ++failures;
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
