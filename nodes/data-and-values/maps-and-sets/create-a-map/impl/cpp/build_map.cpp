// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-map/.

#include "build_map.hpp"

#include <algorithm>
#include <unordered_map>

std::vector<std::pair<std::string, int>> build_map(const std::vector<std::pair<std::string, int>> &pairs, const std::vector<std::string> &remove) {
    std::unordered_map<std::string, int> map;
    for (const auto &[key, value] : pairs) {
        // insert_or_assign adds the key, or replaces the value of a key that is there.
        map.insert_or_assign(key, value);
    }
    for (const auto &key : remove) {
        // erase returns 0 for a key that is not there.
        map.erase(key);
    }
    std::vector<std::pair<std::string, int>> out(map.begin(), map.end());
    // std::string compares its chars, so the keys sort by their bytes.
    std::sort(out.begin(), out.end());
    return out;
}
