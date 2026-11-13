// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-combinations-of-a-list/.

#include "combinations.hpp"

#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

using List = std::vector<std::int32_t>;
using Lists = std::vector<List>;

static void check(std::string_view name, const List &items, std::size_t k, const Lists &want) {
    if (combinations(items, k) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("two-of-four", List{1, 2, 3, 4}, 2, Lists{{1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}});
    check("three-of-four", List{1, 2, 3, 4}, 3, Lists{{1, 2, 3}, {1, 2, 4}, {1, 3, 4}, {2, 3, 4}});
    check("zero-k", List{1, 2}, 0, Lists{List{}});
    check("empty", List{}, 0, Lists{List{}});
    check("k-too-large", List{1, 2}, 3, Lists{});
    check("all", List{1, 2, 3}, 3, Lists{{1, 2, 3}});
    check("duplicates", List{1, 1, 2}, 2, Lists{{1, 1}, {1, 2}, {1, 2}});
    check("unsorted", List{3, 1, 2}, 2, Lists{{3, 1}, {3, 2}, {1, 2}});
    check("extremes", List{INT32_MIN, INT32_MAX}, 1, Lists{{INT32_MIN}, {INT32_MAX}});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
