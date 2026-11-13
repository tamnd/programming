// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-combinations-of-a-list/.

#include "combinations.hpp"

#include <algorithm>

std::vector<std::vector<std::int32_t>> combinations(const std::vector<std::int32_t> &items, std::size_t k) {
    std::vector<std::vector<std::int32_t>> out;
    if (k > items.size()) {
        return out;
    }
    // mask marks the chosen positions: k ones, then zeros. prev_permutation
    // goes from 1100 to 1010 and on to 0011, so the chosen positions come in
    // lexicographic order.
    std::vector<char> mask(items.size(), 0);
    std::fill_n(mask.begin(), k, char{1});
    do {
        std::vector<std::int32_t> pick;
        pick.reserve(k);
        for (std::size_t i = 0; i < items.size(); i++) {
            if (mask[i]) {
                pick.push_back(items[i]);
            }
        }
        out.push_back(std::move(pick));
    } while (std::prev_permutation(mask.begin(), mask.end()));
    return out;
}
