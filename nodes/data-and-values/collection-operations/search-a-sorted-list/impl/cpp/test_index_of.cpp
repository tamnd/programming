// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-a-sorted-list/.

#include "index_of.hpp"

#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<int> &items, int target, long want) {
    if (index_of(items, target) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("middle", std::vector<int>{1, 3, 5, 7, 9}, 5, 2);
    check("first", std::vector<int>{1, 3, 5, 7, 9}, 1, 0);
    check("last", std::vector<int>{1, 3, 5, 7, 9}, 9, 4);
    check("missing", std::vector<int>{1, 3, 5, 7, 9}, 4, -1);
    check("below", std::vector<int>{1, 3, 5}, 0, -1);
    check("above", std::vector<int>{1, 3, 5}, 6, -1);
    check("empty", std::vector<int>{}, 1, -1);
    check("single-hit", std::vector<int>{7}, 7, 0);
    check("single-miss", std::vector<int>{7}, 8, -1);
    check("duplicates", std::vector<int>{1, 2, 2, 2, 3}, 2, 1);
    check("all-same", std::vector<int>{4, 4, 4, 4}, 4, 0);
    check("negative", std::vector<int>{-9, -5, -1, 0}, -5, 1);
    check("extremes", std::vector<int>{(-2147483647 - 1), 0, 2147483647}, 2147483647, 2);
    check("min-target", std::vector<int>{(-2147483647 - 1), 0, 2147483647}, (-2147483647 - 1), 0);
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
