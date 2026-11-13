// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-two-dimensional-array/.

#include "grid.hpp"

#include <cstddef>
#include <vector>

std::vector<std::vector<int>> grid(std::size_t rows, std::size_t cols, int value) {
    // The outer vector holds rows copies of one row, and each copy is a
    // separate vector.
    return std::vector<std::vector<int>>(rows, std::vector<int>(cols, value));
}
