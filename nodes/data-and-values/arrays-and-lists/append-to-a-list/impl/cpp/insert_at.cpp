// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-list/.

#include "insert_at.hpp"

#include <cstddef>
#include <span>
#include <stdexcept>
#include <vector>

std::vector<int> insert_at(std::span<const int> items, std::size_t index, int value) {
    if (index > items.size()) {
        throw std::out_of_range("insert_at: index is past the end");
    }
    std::vector<int> out;
    out.reserve(items.size() + 1);
    out.assign(items.begin(), items.end());
    // insert puts the value before the position, so out.end() appends.
    out.insert(out.begin() + static_cast<std::ptrdiff_t>(index), value);
    return out;
}
