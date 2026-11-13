// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/flatten-a-nested-list/.

#include "flatten.hpp"

#include <cstddef>

std::vector<std::int32_t> flatten(const std::vector<std::vector<std::int32_t>> &lists) {
    std::size_t total = 0;
    for (const auto &xs : lists) {
        total += xs.size();
    }
    std::vector<std::int32_t> out;
    out.reserve(total);
    for (const auto &xs : lists) {
        out.insert(out.end(), xs.begin(), xs.end());
    }
    return out;
}
