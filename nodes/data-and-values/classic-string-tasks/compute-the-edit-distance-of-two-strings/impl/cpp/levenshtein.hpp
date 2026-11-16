// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-edit-distance-of-two-strings/.

#ifndef LEVENSHTEIN_HPP
#define LEVENSHTEIN_HPP

#include <algorithm>
#include <cstddef>
#include <numeric>
#include <string_view>
#include <utility>
#include <vector>

// Returns the minimum number of one-character insertions, deletions and
// substitutions that change a into b. Two rows of the table are kept.
inline std::size_t levenshtein(std::string_view a, std::string_view b) {
    std::vector<std::size_t> prev(b.size() + 1);
    std::vector<std::size_t> cur(b.size() + 1);
    std::iota(prev.begin(), prev.end(), std::size_t{0});
    for (std::size_t i = 1; i <= a.size(); ++i) {
        cur[0] = i;
        for (std::size_t j = 1; j <= b.size(); ++j) {
            const std::size_t cost = a[i - 1] == b[j - 1] ? 0 : 1;
            cur[j] = std::min({prev[j] + 1, cur[j - 1] + 1, prev[j - 1] + cost});
        }
        std::swap(prev, cur);
    }
    return prev[b.size()];
}

#endif
