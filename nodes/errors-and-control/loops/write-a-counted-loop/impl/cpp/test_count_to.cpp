// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-counted-loop/.

#include "count_to.hpp"

#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, int n, const std::vector<int> &want) {
    if (count_to(n) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("ten", 10, std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    check("three", 3, std::vector<int>{1, 2, 3});
    check("zero", 0, std::vector<int>{});
    check("one", 1, std::vector<int>{1});
    check("two", 2, std::vector<int>{1, 2});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
