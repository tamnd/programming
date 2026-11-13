// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-items-of-each-kind/.

#include "count_items.hpp"

#include <map>

std::vector<std::pair<int, std::size_t>> count_items(std::span<const int> items) {
    // A std::map keeps its keys in ascending order.
    std::map<int, std::size_t> counts;
    for (int x : items) {
        ++counts[x];
    }
    return {counts.begin(), counts.end()};
}
