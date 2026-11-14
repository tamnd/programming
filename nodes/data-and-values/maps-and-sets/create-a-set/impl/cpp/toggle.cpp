// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-set/.

#include "toggle.hpp"

#include <algorithm>
#include <unordered_set>

std::vector<std::string> toggle(const std::vector<std::string> &words) {
    std::unordered_set<std::string> seen;
    for (const auto &word : words) {
        // erase returns the number of items that it removed, 0 or 1.
        if (seen.erase(word) == 0) {
            seen.insert(word);
        }
    }
    std::vector<std::string> out(seen.begin(), seen.end());
    std::ranges::sort(out);
    return out;
}
