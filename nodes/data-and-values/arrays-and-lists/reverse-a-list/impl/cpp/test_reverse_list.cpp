// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-list/.

#include "reverse_list.hpp"

#include <climits>
#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<int> &items, const std::vector<int> &want) {
    if (reverse_list(items) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("three", std::vector<int>{1, 2, 3}, std::vector<int>{3, 2, 1});
    check("four", std::vector<int>{4, 7, 1, 9}, std::vector<int>{9, 1, 7, 4});
    check("empty", std::vector<int>{}, std::vector<int>{});
    check("single", std::vector<int>{5}, std::vector<int>{5});
    check("two", std::vector<int>{1, 2}, std::vector<int>{2, 1});
    check("duplicates", std::vector<int>{1, 1, 2}, std::vector<int>{2, 1, 1});
    check("palindrome", std::vector<int>{1, 2, 1}, std::vector<int>{1, 2, 1});
    check("negative", std::vector<int>{-1, 0, -3}, std::vector<int>{-3, 0, -1});
    check("limit", std::vector<int>{INT_MAX, INT_MIN, 0}, std::vector<int>{0, INT_MIN, INT_MAX});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
