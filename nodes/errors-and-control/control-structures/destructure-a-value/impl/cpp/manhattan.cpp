// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/destructure-a-value/.

#include "manhattan.hpp"

#include <cstdlib>

int manhattan(const std::array<int, 2> &p, const std::array<int, 2> &q) {
    // Structured bindings give a name to each element of the array.
    const auto [x1, y1] = p;
    const auto [x2, y2] = q;
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}
