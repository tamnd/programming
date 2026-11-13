// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list/.

#include "sort_ints.hpp"

#include <algorithm>
#include <vector>

std::vector<int> sort_ints(std::vector<int> items) {
    std::ranges::sort(items);
    return items;
}
