// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/partition-a-list/.

#include "partition_odd.hpp"

#include <algorithm>
#include <iterator>

std::pair<std::vector<int>, std::vector<int>> partition_odd(std::span<const int> items) {
    std::vector<int> odd;
    std::vector<int> even;
    std::ranges::partition_copy(items, std::back_inserter(odd), std::back_inserter(even),
                                [](int x) { return x % 2 != 0; });
    return {odd, even};
}
