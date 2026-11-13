// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-items-of-each-kind/.

#include "count_items.hpp"

#include <cstddef>
#include <iostream>
#include <string_view>
#include <utility>
#include <vector>

static int failures = 0;

using Pairs = std::vector<std::pair<int, std::size_t>>;

static void check(std::string_view name, const std::vector<int> &items, const Pairs &want) {
    if (count_items(items) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("mixed", std::vector<int>{3, 1, 3, 2, 1, 3}, Pairs{{1, 2}, {2, 1}, {3, 3}});
    check("same", std::vector<int>{5, 5, 5}, Pairs{{5, 3}});
    check("empty", std::vector<int>{}, Pairs{});
    check("single", std::vector<int>{7}, Pairs{{7, 1}});
    check("negative", std::vector<int>{-1, 2, -1, 0}, Pairs{{-1, 2}, {0, 1}, {2, 1}});
    check("distinct", std::vector<int>{4, 2, 9}, Pairs{{2, 1}, {4, 1}, {9, 1}});
    check("order", std::vector<int>{9, 8, 9}, Pairs{{8, 1}, {9, 2}});
    check("zero", std::vector<int>{0, 0}, Pairs{{0, 2}});
    check("extremes", std::vector<int>{2147483647, (-2147483647 - 1), 2147483647}, Pairs{{(-2147483647 - 1), 1}, {2147483647, 2}});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
