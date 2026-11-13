// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-list/.

#include "reverse_list.hpp"

#include <span>
#include <vector>

std::vector<int> reverse_list(std::span<const int> items) {
    // The reverse iterators walk the items from the end to the start.
    return std::vector<int>(items.rbegin(), items.rend());
}
