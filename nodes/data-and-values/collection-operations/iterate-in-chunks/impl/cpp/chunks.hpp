// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-in-chunks/.

#pragma once

#include <cstddef>
#include <span>
#include <vector>

// chunks returns the chunks of items with size items each; the last chunk can
// be shorter. size must be at least 1.
std::vector<std::vector<int>> chunks(std::span<const int> items, std::size_t size);
