// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-nested-loops/.

#pragma once

#include <utility>
#include <vector>

// find_in_grid returns the row and the column of the first cell that equals
// target, or {-1, -1} when no cell matches.
std::pair<int, int> find_in_grid(const std::vector<std::vector<int>> &grid, int target);
