// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-custom-key-in-a-map/.

#pragma once

#include <tuple>
#include <utility>
#include <vector>

// count_points returns (x, y, count) for each distinct point, in order of x,
// then y.
std::vector<std::tuple<int, int, int>> count_points(const std::vector<std::pair<int, int>> &points);
