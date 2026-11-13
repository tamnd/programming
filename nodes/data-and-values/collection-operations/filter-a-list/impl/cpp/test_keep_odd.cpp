// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-a-list/.

#include "keep_odd.hpp"

#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<int> &items, const std::vector<int> &want) {
    if (keep_odd(items) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("mixed", std::vector<int>{1, 2, 3, 4, 5}, {1, 3, 5});
    check("tens", std::vector<int>{10, 15, 20, 25}, {15, 25});
    check("empty", std::vector<int>{}, std::vector<int>{});
    check("none-odd", std::vector<int>{2, 4, 6}, {});
    check("all-odd", std::vector<int>{1, 3, 5}, {1, 3, 5});
    check("negative", std::vector<int>{-3, -2, -1, 0}, {-3, -1});
    check("zero", std::vector<int>{0}, {});
    check("order", std::vector<int>{9, 1, 7}, {9, 1, 7});
    check("extremes", std::vector<int>{(-2147483647 - 1), 2147483647}, {2147483647});
    check("duplicates", std::vector<int>{3, 3, 2, 3}, {3, 3, 3});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
