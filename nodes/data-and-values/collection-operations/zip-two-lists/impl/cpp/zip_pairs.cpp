// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/zip-two-lists/.

#include "zip_pairs.hpp"

#include <ranges>

std::vector<std::pair<int, int>> zip_pairs(std::span<const int> a, std::span<const int> b) {
    std::vector<std::pair<int, int>> out;
    // views::zip stops at the end of the shorter range.
    for (auto [x, y] : std::views::zip(a, b)) {
        out.emplace_back(x, y);
    }
    return out;
}
