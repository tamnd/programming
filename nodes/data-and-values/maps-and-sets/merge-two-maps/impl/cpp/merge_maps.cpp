// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/merge-two-maps/.

#include "merge_maps.hpp"

#include <map>

std::vector<std::pair<std::string, int>> merge_maps(const std::unordered_map<std::string, int> &base, const std::unordered_map<std::string, int> &over) {
    // A std::map gives its entries in key order.
    std::map<std::string, int> merged(base.begin(), base.end());
    for (const auto &[key, value] : over) {
        // insert and merge keep the value that is there. insert_or_assign replaces it.
        merged.insert_or_assign(key, value);
    }
    return {merged.begin(), merged.end()};
}
