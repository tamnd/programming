// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-a-step/.

#include "count_by.hpp"

#include <climits>
#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, int start, int stop, int step, const std::vector<int> &want) {
    if (count_by(start, stop, step) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("by-two", 2, 8, 2, {2, 4, 6, 8});
    check("down", 10, 0, -1, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0});
    check("uneven", 1, 10, 3, {1, 4, 7, 10});
    check("past-stop", 0, 9, 4, {0, 4, 8});
    check("down-by-three", 10, 1, -3, {10, 7, 4, 1});
    check("empty-up", 5, 1, 1, {});
    check("empty-down", 1, 5, -1, {});
    check("single", 3, 3, 7, {3});
    check("negative", -3, 3, 2, {-3, -1, 1, 3});
    check("limit-up", 2147483640, INT_MAX, 5, {2147483640, 2147483645});
    check("limit-down", -2147483640, INT_MIN, -5, {-2147483640, -2147483645});
    check("big-step", 0, 10, INT_MAX, {0});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
