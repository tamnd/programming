// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-a-list/.

#pragma once

#include <span>
#include <vector>

// keep_odd returns a new vector of the odd items, in their order.
std::vector<int> keep_odd(std::span<const int> items);
