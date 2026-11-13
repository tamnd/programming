// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

#include "contains.hpp"

#include <algorithm>
#include <span>

bool contains(std::span<const int> items, int value) {
    // std::ranges::contains is new in C++23. Before it, compare
    // std::find(first, last, value) with last.
    return std::ranges::contains(items, value);
}
