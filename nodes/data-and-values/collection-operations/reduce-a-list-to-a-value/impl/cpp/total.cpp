// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reduce-a-list-to-a-value/.

#include "total.hpp"

#include <numeric>
#include <span>

int total(std::span<const int> items) {
    // The type of the initial value is the type of the accumulator, so 0LL
    // gives a long long.
    return static_cast<int>(std::accumulate(items.begin(), items.end(), 0LL));
}
