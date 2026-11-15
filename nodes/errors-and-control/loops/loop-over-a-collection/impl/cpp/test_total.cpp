// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-a-collection/.

#include "total.hpp"

#include <climits>
#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<int> &items, long long want) {
    if (total(items) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("three", {1, 2, 3}, 6);
    check("mixed", {10, -4, 7, 0, 3}, 16);
    check("empty", {}, 0);
    check("single", {7}, 7);
    check("negative", {-5, 2, -1}, -4);
    check("limit", {INT_MAX, INT_MAX, 2}, 4294967296LL);
    check("limit-low", {INT_MIN, INT_MIN}, -4294967296LL);
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
