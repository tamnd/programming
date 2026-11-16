// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/take-the-first-n-items/.

#pragma once

#include <cstddef>
#include <span>
#include <vector>

// take returns a copy of the first n items; a shorter span gives all its items.
std::vector<int> take(std::span<const int> items, std::size_t n);

// take_naturals returns the first n items of the infinite sequence start,
// start + 1, ...
std::vector<int> take_naturals(int start, std::size_t n);
