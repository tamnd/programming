// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-nested-loops/.

#include "find_in_grid.hpp"

#include <iostream>
#include <string_view>
#include <utility>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<std::vector<int>> &grid, int target,
                  std::pair<int, int> want) {
    if (find_in_grid(grid, target) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("middle", {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 5, {1, 1});
    check("first-twenty", {{3, 8, 12}, {5, 20, 20}, {20, 1, 4}}, 20, {1, 1});
    check("first-cell", {{9, 1}, {2, 3}}, 9, {0, 0});
    check("last-cell", {{1, 2}, {3, 9}}, 9, {1, 1});
    check("row-major", {{1, 2, 9}, {9, 3, 4}}, 9, {0, 2});
    check("missing", {{1, 2}, {3, 4}}, 9, {-1, -1});
    check("empty-grid", {}, 1, {-1, -1});
    check("empty-rows", {{}, {}, {5}}, 5, {2, 0});
    check("ragged", {{1}, {2, 3, 4}, {5, 6}}, 6, {2, 1});
    check("negative", {{-1, -2}, {-3, -4}}, -3, {1, 0});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
