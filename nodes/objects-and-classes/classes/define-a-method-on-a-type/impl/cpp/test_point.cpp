// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-method-on-a-type/.

#include "point.hpp"

#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, int x, int y, int dx, int dy, const std::vector<int> &want) {
    if (point_report(x, y, dx, dy) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("right-and-up", 1, 2, 3, 4, {4, 6, 7, 10});
    check("mixed-signs", 5, -1, -2, 3, {3, 2, 5, 5});
    check("zero-move", 7, -3, 0, 0, {7, -3, 0, 10});
    check("negative", -2, -5, -3, -1, {-5, -6, 4, 11});
    check("origin", 2, 3, -2, -3, {0, 0, 5, 0});
    check("limit", 100000000, -100000000, 100000000, -100000000, {200000000, -200000000, 200000000, 400000000});

    const Point p{1, 2};
    const Point q = p.translate(3, 4);
    if (!(p == Point{1, 2}) || !(q == Point{4, 6})) {
        std::cerr << "case another-point failed\n";
        failures++;
    }
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
