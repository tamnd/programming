// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-class/.

#include "rectangle.hpp"

#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, int width, int height, int factor, const std::vector<int> &want) {
    if (rectangle_report(width, height, factor) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("three-by-four", 3, 4, 2, {12, 14, 48, 28});
    check("square", 5, 5, 3, {25, 20, 225, 60});
    check("zero-side", 0, 7, 2, {0, 14, 0, 28});
    check("zero-factor", 3, 4, 0, {12, 14, 0, 0});
    check("identity", 3, 4, 1, {12, 14, 12, 14});
    check("limit", 1000, 1000, 46, {1000000, 4000, 2116000000, 184000});

    Rectangle a(1, 2);
    Rectangle b(1, 2);
    a.scale(3);
    if (a.to_string() != "Rectangle(3, 6)" || b.to_string() != "Rectangle(1, 2)") {
        std::cerr << "case own-fields failed\n";
        failures++;
    }
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
