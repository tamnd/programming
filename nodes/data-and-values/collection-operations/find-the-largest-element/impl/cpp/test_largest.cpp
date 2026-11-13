// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-largest-element/.

#include "largest.hpp"

#include <iostream>
#include <stdexcept>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<int> &items, int want) {
    if (largest(items) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("digits", std::vector<int>{3, 1, 4, 1, 5}, 5);
    check("middle", std::vector<int>{10, 20, 15}, 20);
    check("single", std::vector<int>{7}, 7);
    check("all-negative", std::vector<int>{-3, -1, -2}, -1);
    check("first", std::vector<int>{9, 2, 3}, 9);
    check("last", std::vector<int>{1, 2, 9}, 9);
    check("duplicates", std::vector<int>{5, 9, 9, 1}, 9);
    check("all-same", std::vector<int>{4, 4, 4}, 4);
    check("zero", std::vector<int>{-5, 0, -1}, 0);
    check("extremes", std::vector<int>{(-2147483647 - 1), 2147483647, 0}, 2147483647);
    check("min-only", std::vector<int>{(-2147483647 - 1)}, (-2147483647 - 1));
    try {
        largest(std::vector<int>{});
        std::cerr << "case empty: no exception\n";
        failures++;
    } catch (const std::invalid_argument &) {
    }
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
