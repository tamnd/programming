// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/group-items-by-a-key/.

#include "group_by_length.hpp"

#include <map>

std::vector<std::pair<std::size_t, std::vector<std::string>>> group_by_length(const std::vector<std::string> &words) {
    // A std::map keeps its keys in ascending order.
    std::map<std::size_t, std::vector<std::string>> groups;
    for (const auto &w : words) {
        groups[w.size()].push_back(w);
    }
    return {groups.begin(), groups.end()};
}
