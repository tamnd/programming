// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/slice-a-list/.

#pragma once

#include <cstddef>
#include <span>
#include <vector>

// slice_list returns a new vector of items[start] up to, but not including,
// items[stop]. It throws std::out_of_range when start > stop or stop > size.
std::vector<int> slice_list(std::span<const int> items, std::size_t start, std::size_t stop);
