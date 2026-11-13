// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-largest-element/.

#include "largest.hpp"

#include <algorithm>
#include <span>
#include <stdexcept>

int largest(std::span<const int> items) {
    if (items.empty()) {
        throw std::invalid_argument("largest: the span is empty");
    }
    return *std::ranges::max_element(items);
}
