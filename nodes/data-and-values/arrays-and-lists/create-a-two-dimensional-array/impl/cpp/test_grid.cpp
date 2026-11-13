// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-two-dimensional-array/.

#include "grid.hpp"

#include <climits>
#include <cstddef>
#include <iostream>
#include <string_view>
#include <vector>

using Grid = std::vector<std::vector<int>>;

static int failures = 0;

static void check(std::string_view name, std::size_t rows, std::size_t cols, int value, const Grid &want) {
    Grid got = grid(rows, cols, value);
    bool ok = got == want;
    if (ok && rows >= 2 && cols >= 1) {
        got[0][0] = value ^ 1;
        ok = got[1][0] == value;
    }
    if (!ok) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("two-by-three", 2, 3, 0, Grid{std::vector<int>{0, 0, 0}, std::vector<int>{0, 0, 0}});
    check("three-by-one", 3, 1, 7, Grid{std::vector<int>{7}, std::vector<int>{7}, std::vector<int>{7}});
    check("no-rows", 0, 3, 1, Grid{});
    check("no-cols", 2, 0, 1, Grid{std::vector<int>{}, std::vector<int>{}});
    check("one-cell", 1, 1, 5, Grid{std::vector<int>{5}});
    check("one-row", 1, 4, 2, Grid{std::vector<int>{2, 2, 2, 2}});
    check("one-col", 4, 1, 2, Grid{std::vector<int>{2}, std::vector<int>{2}, std::vector<int>{2}, std::vector<int>{2}});
    check("negative", 2, 2, -3, Grid{std::vector<int>{-3, -3}, std::vector<int>{-3, -3}});
    check("min-int", 2, 2, INT_MIN, Grid{std::vector<int>{INT_MIN, INT_MIN}, std::vector<int>{INT_MIN, INT_MIN}});
    check("max-int", 1, 2, INT_MAX, Grid{std::vector<int>{INT_MAX, INT_MAX}});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
