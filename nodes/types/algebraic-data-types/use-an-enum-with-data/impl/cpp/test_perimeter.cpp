// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-an-enum-with-data/.

#include "perimeter.hpp"

#include <iostream>
#include <optional>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view label, std::string_view kind, const std::vector<int> &sides,
                  std::optional<int> want) {
    if (perimeter(kind, sides) != want) {
        std::cerr << "case " << label << " failed\n";
        failures++;
    }
}

int main() {
    check("square", "square", {3}, 12);
    check("rectangle", "rectangle", {2, 5}, 14);
    check("triangle", "triangle", {3, 4, 5}, 12);
    check("unknown-kind", "circle", {2}, std::nullopt);
    check("wrong-case", "Square", {3}, std::nullopt);
    check("too-few-sides", "rectangle", {4}, std::nullopt);
    check("too-many-sides", "square", {3, 3}, std::nullopt);
    check("no-sides", "triangle", {}, std::nullopt);
    check("zero-side", "square", {0}, std::nullopt);
    check("negative-side", "rectangle", {-2, 5}, std::nullopt);
    check("not-a-triangle", "triangle", {1, 2, 3}, std::nullopt);
    check("largest", "triangle", {1000000, 1000000, 1000000}, 3000000);
    if (make_shape("rectangle", {2, 5}) != std::optional<Shape>{Rectangle{2, 5}}) {
        std::cerr << "case variant-data failed\n";
        failures++;
    }
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
