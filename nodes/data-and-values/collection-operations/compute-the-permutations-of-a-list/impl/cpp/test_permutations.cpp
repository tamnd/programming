// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-permutations-of-a-list/.

#include "permutations.hpp"

#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

using List = std::vector<std::int32_t>;
using Lists = std::vector<List>;

static void check(std::string_view name, const List &items, const Lists &want) {
    if (permutations(items) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("three", List{1, 2, 3}, Lists{{1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1}});
    check("two", List{5, 6}, Lists{{5, 6}, {6, 5}});
    check("empty", List{}, Lists{List{}});
    check("single", List{7}, Lists{{7}});
    check("duplicates", List{1, 1, 2}, Lists{{1, 1, 2}, {1, 2, 1}, {1, 1, 2}, {1, 2, 1}, {2, 1, 1}, {2, 1, 1}});
    check("unsorted", List{3, 1, 2}, Lists{{3, 1, 2}, {3, 2, 1}, {1, 3, 2}, {1, 2, 3}, {2, 3, 1}, {2, 1, 3}});
    check("extremes", List{INT32_MIN, INT32_MAX}, Lists{{INT32_MIN, INT32_MAX}, {INT32_MAX, INT32_MIN}});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
