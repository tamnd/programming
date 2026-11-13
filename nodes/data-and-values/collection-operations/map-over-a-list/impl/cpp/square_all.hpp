// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/map-over-a-list/.

#pragma once

#include <span>
#include <vector>

// square_all returns a new vector with the square of each item, in the same
// order.
std::vector<int> square_all(std::span<const int> items);
