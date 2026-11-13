// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/slice-a-list/.

#include "slice_list.hpp"

#include <cstddef>
#include <span>
#include <stdexcept>
#include <vector>

std::vector<int> slice_list(std::span<const int> items, std::size_t start, std::size_t stop) {
    if (start > stop || stop > items.size()) {
        throw std::out_of_range("slice_list: the range is out of bounds");
    }
    // subspan takes an offset and a count. The vector copies the items.
    const auto part = items.subspan(start, stop - start);
    return std::vector<int>(part.begin(), part.end());
}
