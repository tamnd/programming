// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/take-the-first-n-items/.

#include "take.hpp"

#include <cstddef>
#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

using Ints = std::vector<int>;

static void check(std::string_view name, const Ints &got, const Ints &want) {
    if (got != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("first-three", take(Ints{1, 2, 3, 4, 5}, 3), Ints{1, 2, 3});
    check("first-two", take(Ints{10, 20, 30, 40}, 2), Ints{10, 20});
    check("zero", take(Ints{1, 2}, 0), Ints{});
    check("empty", take(Ints{}, 3), Ints{});
    check("more-than-length", take(Ints{1, 2}, 5), Ints{1, 2});
    check("equal-length", take(Ints{4, 5, 6}, 3), Ints{4, 5, 6});
    check("one", take(Ints{9, 8}, 1), Ints{9});
    check("negative", take(Ints{-1, -2, -3}, 2), Ints{-1, -2});
    check("extremes", take(Ints{(-2147483647 - 1), 2147483647, 0}, 2), Ints{(-2147483647 - 1), 2147483647});
    check("from-zero", take_naturals(0, 5), Ints{0, 1, 2, 3, 4});
    check("from-ten", take_naturals(10, 3), Ints{10, 11, 12});
    check("none", take_naturals(7, 0), Ints{});
    check("one", take_naturals(0, 1), Ints{0});
    check("large-start", take_naturals(999998, 2), Ints{999998, 999999});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
