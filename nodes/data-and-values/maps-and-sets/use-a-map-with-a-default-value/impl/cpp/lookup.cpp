// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.

#include "lookup.hpp"

std::vector<int> lookup(const std::unordered_map<std::string, int> &table, const std::vector<std::string> &keys, int fallback) {
    std::vector<int> out;
    out.reserve(keys.size());
    for (const auto &key : keys) {
        // find does not add the key. operator[] would add it with the value 0,
        // and at would throw std::out_of_range.
        auto it = table.find(key);
        out.push_back(it == table.end() ? fallback : it->second);
    }
    return out;
}
