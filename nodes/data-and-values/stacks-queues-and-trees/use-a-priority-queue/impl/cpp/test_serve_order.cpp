// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-priority-queue/.

#include "serve_order.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view label, const std::vector<std::int32_t> &priorities,
                  const std::vector<std::size_t> &want) {
    if (serve_order(priorities) != want) {
        std::cerr << "case " << label << " failed\n";
        failures++;
    }
}

int main() {
    constexpr auto hi = std::numeric_limits<std::int32_t>::max();
    constexpr auto lo = std::numeric_limits<std::int32_t>::min();
    check("example", {2, 5, 1, 5}, {1, 3, 0, 2});
    check("longer", {3, 1, 4, 1, 5, 9, 2, 6}, {5, 7, 4, 2, 0, 6, 1, 3});
    check("empty", {}, {});
    check("one", {7}, {0});
    check("ties", {4, 4, 4, 4}, {0, 1, 2, 3});
    check("negative", {-3, -1, -2}, {1, 2, 0});
    check("int32-extremes", {lo, hi, 0, lo}, {1, 2, 0, 3});
    check("sorted", {1, 2, 3}, {2, 1, 0});
    check("reversed", {3, 2, 1}, {0, 1, 2});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
