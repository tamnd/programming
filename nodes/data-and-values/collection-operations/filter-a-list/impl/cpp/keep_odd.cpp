// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-a-list/.

#include "keep_odd.hpp"

#include <algorithm>
#include <iterator>
#include <span>
#include <vector>

std::vector<int> keep_odd(std::span<const int> items) {
    std::vector<int> out;
    // Use != 0, not == 1: the remainder of a negative item can be negative.
    std::ranges::copy_if(items, std::back_inserter(out), [](int x) { return x % 2 != 0; });
    return out;
}
