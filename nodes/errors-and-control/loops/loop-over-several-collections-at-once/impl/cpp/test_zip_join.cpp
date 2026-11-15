// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-several-collections-at-once/.

#include "zip_join.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<std::string> &lower, const std::vector<std::string> &upper,
                  const std::vector<int> &digits, const std::vector<std::string> &want) {
    if (zip_join(lower, upper, digits) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("rosetta", {"a", "b", "c"}, {"A", "B", "C"}, {1, 2, 3}, {"aA1", "bB2", "cC3"});
    check("words", {"x", "y"}, {"X", "Y"}, {10, 20}, {"xX10", "yY20"});
    check("shorter-first", {"a"}, {"A", "B"}, {1, 2, 3}, {"aA1"});
    check("shorter-last", {"a", "b", "c"}, {"A", "B", "C"}, {1}, {"aA1"});
    check("one-empty", {"a", "b"}, {}, {1, 2}, {});
    check("all-empty", {}, {}, {}, {});
    check("negative", {"a"}, {"A"}, {-5}, {"aA-5"});
    check("empty-strings", {"", "b"}, {"A", ""}, {1, 2}, {"A1", "b2"});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
