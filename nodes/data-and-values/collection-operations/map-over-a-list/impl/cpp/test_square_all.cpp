// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/map-over-a-list/.

#include "square_all.hpp"

#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<int> &items, const std::vector<int> &want) {
    if (square_all(items) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("small", std::vector<int>{1, 2, 3}, {1, 4, 9});
    check("tens", std::vector<int>{5, 10}, {25, 100});
    check("empty", std::vector<int>{}, std::vector<int>{});
    check("single", std::vector<int>{7}, {49});
    check("negative", std::vector<int>{-3, -1}, {9, 1});
    check("zero", std::vector<int>{0}, {0});
    check("limit", std::vector<int>{46340, -46340}, {2147395600, 2147395600});
    check("order", std::vector<int>{3, 1, 2}, {9, 1, 4});
    check("duplicates", std::vector<int>{2, 2}, {4, 4});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
