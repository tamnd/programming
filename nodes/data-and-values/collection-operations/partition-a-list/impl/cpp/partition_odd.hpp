// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/partition-a-list/.

#pragma once

#include <span>
#include <utility>
#include <vector>

// partition_odd returns the odd items and the even items, each in input order.
std::pair<std::vector<int>, std::vector<int>> partition_odd(std::span<const int> items);
