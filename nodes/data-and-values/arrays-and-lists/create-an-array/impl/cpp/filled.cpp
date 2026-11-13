// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-an-array/.

#include "filled.hpp"

#include <cstddef>
#include <vector>

std::vector<int> filled(std::size_t n, int value) {
    // Parentheses call the (count, value) constructor. Braces would make a
    // vector of the two items n and value.
    return std::vector<int>(n, value);
}
