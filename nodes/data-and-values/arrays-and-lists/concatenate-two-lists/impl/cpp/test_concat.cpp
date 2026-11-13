// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-two-lists/.

#include "concat.hpp"

#include <climits>
#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<int> &a, const std::vector<int> &b, const std::vector<int> &want) {
    if (concat(a, b) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("two-pairs", std::vector<int>{1, 2}, std::vector<int>{3, 4}, std::vector<int>{1, 2, 3, 4});
    check("uneven", std::vector<int>{5}, std::vector<int>{6, 7, 8}, std::vector<int>{5, 6, 7, 8});
    check("both-empty", std::vector<int>{}, std::vector<int>{}, std::vector<int>{});
    check("left-empty", std::vector<int>{}, std::vector<int>{1, 2}, std::vector<int>{1, 2});
    check("right-empty", std::vector<int>{1, 2}, std::vector<int>{}, std::vector<int>{1, 2});
    check("duplicates", std::vector<int>{1, 1}, std::vector<int>{1}, std::vector<int>{1, 1, 1});
    check("order", std::vector<int>{3, 1}, std::vector<int>{2}, std::vector<int>{3, 1, 2});
    check("limit", std::vector<int>{INT_MAX}, std::vector<int>{INT_MIN}, std::vector<int>{INT_MAX, INT_MIN});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
