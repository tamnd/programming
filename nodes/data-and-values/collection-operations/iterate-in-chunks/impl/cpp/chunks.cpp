// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-in-chunks/.

#include "chunks.hpp"

#include <algorithm>

std::vector<std::vector<int>> chunks(std::span<const int> items, std::size_t size) {
    std::vector<std::vector<int>> out;
    for (std::size_t i = 0; i < items.size(); i += std::min(size, items.size() - i)) {
        // The count must not go past the end of the span.
        auto c = items.subspan(i, std::min(size, items.size() - i));
        out.emplace_back(c.begin(), c.end());
    }
    return out;
}
