// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/take-the-first-n-items/.

#include "take.hpp"

#include <ranges>

std::vector<int> take(std::span<const int> items, std::size_t n) {
    // views::take stops at the end of the span, so n can be larger than the size.
    auto first = items | std::views::take(n);
    return std::vector<int>(first.begin(), first.end());
}

std::vector<int> take_naturals(int start, std::size_t n) {
    std::vector<int> out;
    // views::iota(start) has no end; views::take reads only n values from it.
    for (int i : std::views::iota(start) | std::views::take(n)) {
        out.push_back(i);
    }
    return out;
}
