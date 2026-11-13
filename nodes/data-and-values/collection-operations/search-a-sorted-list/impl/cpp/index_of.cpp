// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-a-sorted-list/.

#include "index_of.hpp"

#include <algorithm>

long index_of(std::span<const int> items, int target) {
    // lower_bound gives the first item that is not less than target.
    auto it = std::ranges::lower_bound(items, target);
    if (it == items.end() || *it != target) {
        return -1;
    }
    return static_cast<long>(it - items.begin());
}
