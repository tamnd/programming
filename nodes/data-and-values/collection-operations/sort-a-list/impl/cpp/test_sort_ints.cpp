// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list/.

#include "sort_ints.hpp"

#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, std::vector<int> items, const std::vector<int> &want) {
    if (sort_ints(items) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("small", std::vector<int>{3, 1, 2}, {1, 2, 3});
    check("five", std::vector<int>{5, 4, 1, 3, 2}, {1, 2, 3, 4, 5});
    check("empty", std::vector<int>{}, std::vector<int>{});
    check("single", std::vector<int>{1}, {1});
    check("sorted", std::vector<int>{1, 2, 3}, {1, 2, 3});
    check("reversed", std::vector<int>{9, 8, 7}, {7, 8, 9});
    check("duplicates", std::vector<int>{2, 1, 2, 1}, {1, 1, 2, 2});
    check("negative", std::vector<int>{-1, -10, 5}, {-10, -1, 5});
    check("digits", std::vector<int>{10, 9, 100, 1}, {1, 9, 10, 100});
    check("extremes", std::vector<int>{2147483647, (-2147483647 - 1), 0}, {(-2147483647 - 1), 0, 2147483647});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
