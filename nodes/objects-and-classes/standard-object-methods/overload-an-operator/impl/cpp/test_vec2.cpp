// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/overload-an-operator/.

#include "vec2.hpp"

#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

using vec2::Vec2;

static int failures = 0;

static void check(std::string_view name, const std::vector<std::int64_t>& xs, const std::vector<std::int64_t>& ys,
                  Vec2 want) {
    if (vec2::vector_sum(xs, ys) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

static void expect(std::string_view name, bool ok) {
    if (!ok) {
        std::cerr << "check " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("two", {1, 3}, {2, 4}, {4, 6});
    check("three", {1, 2, 3}, {10, 20, 30}, {6, 60});
    check("empty", {}, {}, {0, 0});
    check("single", {5}, {-7}, {5, -7});
    check("negative", {-1, -2}, {-3, -4}, {-3, -7});
    check("cancel", {3, -3}, {5, -5}, {0, 0});
    check("limit", {1000000000, 1000000000, 1000000000}, {-1000000000, -1000000000, -1000000000},
          {3000000000, -3000000000});

    Vec2 a{1, 2}, b{3, 4};
    expect("add", a + b == Vec2{4, 6});
    expect("equal", a == Vec2{1, 2} && a != b);
    Vec2 c = a;
    c += b;
    expect("compound", c == Vec2{4, 6});

    if (failures == 0) {
        std::cout << "all tests passed\n";
    }
    return failures == 0 ? 0 : 1;
}
