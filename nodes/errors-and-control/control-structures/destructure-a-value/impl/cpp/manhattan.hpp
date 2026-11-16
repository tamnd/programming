// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/destructure-a-value/.

#pragma once

#include <array>

// manhattan returns the Manhattan distance between the points p = {x, y} and
// q = {x, y}.
int manhattan(const std::array<int, 2> &p, const std::array<int, 2> &q);
