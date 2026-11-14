// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-index-of-a-value/.

#include "index_of.hpp"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <span>

std::ptrdiff_t index_of(std::span<const int> items, int value) {
    // find returns the end when the value is not there, and the distance to
    // the end is the size, not -1.
    const auto it = std::ranges::find(items, value);
    if (it == items.end()) {
        return -1;
    }
    return std::distance(items.begin(), it);
}
