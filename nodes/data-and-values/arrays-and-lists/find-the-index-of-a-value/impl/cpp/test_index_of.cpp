// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-index-of-a-value/.

#include "index_of.hpp"

#include <climits>
#include <cstddef>
#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<int> &items, int value, std::ptrdiff_t want) {
    if (index_of(items, value) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("middle", std::vector<int>{4, 7, 1}, 7, 1);
    check("absent", std::vector<int>{4, 7, 1}, 9, -1);
    check("empty", std::vector<int>{}, 0, -1);
    check("single", std::vector<int>{5}, 5, 0);
    check("single-absent", std::vector<int>{5}, 0, -1);
    check("first", std::vector<int>{7, 8, 9}, 7, 0);
    check("last", std::vector<int>{7, 8, 9}, 9, 2);
    check("duplicates", std::vector<int>{3, 5, 3, 5}, 5, 1);
    check("all-same", std::vector<int>{2, 2, 2}, 2, 0);
    check("negative", std::vector<int>{0, -1, -2}, -2, 2);
    check("limit", std::vector<int>{INT_MAX, INT_MIN}, INT_MIN, 1);
    check("limit-absent", std::vector<int>{INT_MAX}, INT_MIN, -1);
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
