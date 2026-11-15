// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-nested-loops/.

#include "find_in_grid.hpp"

#include <cstddef>
#include <utility>
#include <vector>

std::pair<int, int> find_in_grid(const std::vector<std::vector<int>> &grid, int target) {
    for (std::size_t r = 0; r < grid.size(); r++) {
        for (std::size_t c = 0; c < grid[r].size(); c++) {
            if (grid[r][c] == target) {
                // return leaves both loops. break would leave only the inner one.
                return {static_cast<int>(r), static_cast<int>(c)};
            }
        }
    }
    return {-1, -1};
}
