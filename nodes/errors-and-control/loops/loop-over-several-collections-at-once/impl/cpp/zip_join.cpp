// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-several-collections-at-once/.

#include "zip_join.hpp"

#include <ranges>
#include <string>
#include <vector>

std::vector<std::string> zip_join(const std::vector<std::string> &lower, const std::vector<std::string> &upper,
                                  const std::vector<int> &digits) {
    std::vector<std::string> out;
    // views::zip stops at the end of the shortest range.
    for (auto [a, b, d] : std::views::zip(lower, upper, digits)) {
        out.push_back(a + b + std::to_string(d));
    }
    return out;
}
