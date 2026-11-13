// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rotate-a-list/.

#include "rotate_left.hpp"

#include <cstddef>
#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<int> &items, std::size_t k, const std::vector<int> &want) {
    if (rotate_left(items, k) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("by-two", std::vector<int>{1, 2, 3, 4, 5}, 2, std::vector<int>{3, 4, 5, 1, 2});
    check("by-one", std::vector<int>{1, 2, 3}, 1, std::vector<int>{2, 3, 1});
    check("zero", std::vector<int>{1, 2, 3}, 0, std::vector<int>{1, 2, 3});
    check("full-turn", std::vector<int>{1, 2, 3}, 3, std::vector<int>{1, 2, 3});
    check("more-than-length", std::vector<int>{1, 2, 3}, 4, std::vector<int>{2, 3, 1});
    check("much-larger", std::vector<int>{1, 2, 3}, 2000000000, std::vector<int>{3, 1, 2});
    check("empty", std::vector<int>{}, 5, std::vector<int>{});
    check("empty-zero", std::vector<int>{}, 0, std::vector<int>{});
    check("single", std::vector<int>{7}, 3, std::vector<int>{7});
    check("duplicates", std::vector<int>{1, 1, 2}, 1, std::vector<int>{1, 2, 1});
    check("negative-items", std::vector<int>{-1, -2, -3}, 1, std::vector<int>{-2, -3, -1});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
