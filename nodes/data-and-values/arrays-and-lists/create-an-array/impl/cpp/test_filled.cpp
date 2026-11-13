// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-an-array/.

#include "filled.hpp"

#include <climits>
#include <cstddef>
#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, std::size_t n, int value) {
    if (filled(n, value) != std::vector<int>(n, value) || filled(n, value).size() != n) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("three-sevens", 3, 7);
    check("five-zeros", 5, 0);
    check("empty", 0, 7);
    check("single", 1, 42);
    check("zero", 2, 0);
    check("negative", 2, -1);
    check("max-int", 2, INT_MAX);
    check("min-int", 2, INT_MIN);
    check("ten", 10, 1);
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
