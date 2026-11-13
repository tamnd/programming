// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-two-dimensional-array/.

#pragma once

#include <cstddef>
#include <vector>

// grid returns rows vectors of cols items, each item value.
std::vector<std::vector<int>> grid(std::size_t rows, std::size_t cols, int value);
