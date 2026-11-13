// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rotate-a-list/.

#include "rotate_left.hpp"

#include <algorithm>
#include <cstddef>

std::vector<int> rotate_left(std::span<const int> items, std::size_t k) {
    std::vector<int> v(items.begin(), items.end());
    if (!v.empty()) {
        auto s = static_cast<std::ptrdiff_t>(k % v.size());
        // The middle iterator is the item that becomes the first item.
        std::ranges::rotate(v, v.begin() + s);
    }
    return v;
}
