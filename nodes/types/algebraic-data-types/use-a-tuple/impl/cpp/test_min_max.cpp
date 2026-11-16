// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-tuple/.

#include "min_max.hpp"

#include <cstdint>
#include <iostream>
#include <limits>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view label, const std::vector<std::int32_t> &numbers, std::int32_t lo, std::int32_t hi) {
    auto [got_lo, got_hi] = min_max(numbers); // structured binding
    if (got_lo != lo || got_hi != hi) {
        std::cerr << "case " << label << " failed\n";
        failures++;
    }
}

int main() {
    constexpr auto top = std::numeric_limits<std::int32_t>::max();
    constexpr auto bottom = std::numeric_limits<std::int32_t>::min();
    check("mixed", {3, 1, 2}, 1, 3);
    check("longer", {4, 9, -2, 7, 0}, -2, 9);
    check("one", {5}, 5, 5);
    check("negative", {-3, -1, -7}, -7, -1);
    check("all-same", {2, 2, 2}, 2, 2);
    check("int32-extremes", {top, 0, bottom}, bottom, top);
    check("sorted", {1, 2, 3, 4}, 1, 4);
    check("reversed", {4, 3, 2, 1}, 1, 4);
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
