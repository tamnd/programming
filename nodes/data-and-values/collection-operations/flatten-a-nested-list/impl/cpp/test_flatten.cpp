// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/flatten-a-nested-list/.

#include "flatten.hpp"

#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

using List = std::vector<std::int32_t>;
using Lists = std::vector<List>;

static void check(std::string_view name, const Lists &lists, const List &want) {
    if (flatten(lists) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("three", Lists{{1, 2}, {3}, {4, 5, 6}}, List{1, 2, 3, 4, 5, 6});
    check("two", Lists{{7}, {8, 9}}, List{7, 8, 9});
    check("empty", Lists{}, List{});
    check("empty-inner", Lists{{}, {1}, {}}, List{1});
    check("all-empty", Lists{{}, {}}, List{});
    check("single", Lists{{5}}, List{5});
    check("one-list", Lists{{1, 2, 3}}, List{1, 2, 3});
    check("negative", Lists{{-1, -2}, {3}}, List{-1, -2, 3});
    check("extremes", Lists{{INT32_MIN}, {2147483647, 0}}, List{INT32_MIN, 2147483647, 0});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
