// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

#include "contains.hpp"

#include <climits>
#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<int> &items, int value, bool want) {
    if (contains(items, value) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("present", std::vector<int>{1, 2, 3}, 2, true);
    check("absent", std::vector<int>{1, 2, 3}, 4, false);
    check("empty", std::vector<int>{}, 0, false);
    check("single-yes", std::vector<int>{5}, 5, true);
    check("single-no", std::vector<int>{5}, 0, false);
    check("first", std::vector<int>{7, 8, 9}, 7, true);
    check("last", std::vector<int>{7, 8, 9}, 9, true);
    check("duplicates", std::vector<int>{2, 2, 2}, 2, true);
    check("negative", std::vector<int>{-1, -2, -3}, -2, true);
    check("zero-absent", std::vector<int>{-1, 1}, 0, false);
    check("limit", std::vector<int>{INT_MAX, INT_MIN}, INT_MIN, true);
    check("limit-absent", std::vector<int>{INT_MAX}, INT_MIN, false);
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
