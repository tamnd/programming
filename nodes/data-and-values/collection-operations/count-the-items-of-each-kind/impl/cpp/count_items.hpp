// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-items-of-each-kind/.

#pragma once

#include <cstddef>
#include <span>
#include <utility>
#include <vector>

// count_items returns one (item, count) pair for each distinct item, in
// ascending order of the item.
std::vector<std::pair<int, std::size_t>> count_items(std::span<const int> items);
