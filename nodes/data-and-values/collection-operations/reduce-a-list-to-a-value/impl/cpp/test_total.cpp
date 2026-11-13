// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reduce-a-list-to-a-value/.

#include "total.hpp"

#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<int> &items, int want) {
    if (total(items) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("small", std::vector<int>{1, 2, 3, 4}, 10);
    check("mixed", std::vector<int>{10, -3, 5}, 12);
    check("empty", std::vector<int>{}, 0);
    check("single", std::vector<int>{7}, 7);
    check("negative", std::vector<int>{-1, -2, -3}, -6);
    check("zero-sum", std::vector<int>{5, -5}, 0);
    check("max", std::vector<int>{2147483647}, 2147483647);
    check("min", std::vector<int>{(-2147483647 - 1)}, (-2147483647 - 1));
    check("to-the-limit", std::vector<int>{2147483646, 1}, 2147483647);
    check("partial-overflow", std::vector<int>{2147483647, 1, -1}, 2147483647);
    check("partial-underflow", std::vector<int>{(-2147483647 - 1), -1, 1}, (-2147483647 - 1));
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
