// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-list/.

#pragma once

#include <cstddef>
#include <span>
#include <vector>

// insert_at returns a new vector with value at position index. index
// items.size() appends. It throws std::out_of_range when index > items.size().
std::vector<int> insert_at(std::span<const int> items, std::size_t index, int value);
