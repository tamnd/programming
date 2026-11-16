// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/destructure-a-value/.

#include "manhattan.hpp"

#include <array>
#include <iostream>
#include <string_view>

static int failures = 0;

static void check(std::string_view name, const std::array<int, 2> &p, const std::array<int, 2> &q, int want) {
    if (manhattan(p, q) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("first", {1, 2}, {4, 6}, 7);
    check("on-a-line", {0, 0}, {10, 0}, 10);
    check("down-left", {5, 5}, {2, 1}, 7);
    check("mixed-signs", {3, -1}, {-2, 4}, 10);
    check("same-point", {7, 7}, {7, 7}, 0);
    check("negative", {-3, -4}, {-1, -1}, 5);
    check("large", {1000000, -1000000}, {-1000000, 1000000}, 4000000);
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
