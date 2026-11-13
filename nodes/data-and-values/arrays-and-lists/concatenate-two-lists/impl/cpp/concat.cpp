// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-two-lists/.

#include "concat.hpp"

#include <span>
#include <vector>

std::vector<int> concat(std::span<const int> a, std::span<const int> b) {
    std::vector<int> c;
    c.reserve(a.size() + b.size());
    // insert copies a range. The ranges are not part of c, so the call is safe.
    c.insert(c.end(), a.begin(), a.end());
    c.insert(c.end(), b.begin(), b.end());
    return c;
}
