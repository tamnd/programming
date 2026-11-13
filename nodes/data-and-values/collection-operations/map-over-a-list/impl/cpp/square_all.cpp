// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/map-over-a-list/.

#include "square_all.hpp"

#include <algorithm>
#include <iterator>
#include <span>
#include <vector>

std::vector<int> square_all(std::span<const int> items) {
    std::vector<int> out;
    out.reserve(items.size());
    // back_inserter adds each result to out. A plain out.begin() would write
    // past the end of the empty vector.
    std::ranges::transform(items, std::back_inserter(out), [](int x) { return x * x; });
    return out;
}
