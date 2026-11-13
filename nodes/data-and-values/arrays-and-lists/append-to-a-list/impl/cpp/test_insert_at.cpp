// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-list/.

#include "insert_at.hpp"

#include <climits>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<int> &items, std::size_t index, int value, const std::vector<int> &want) {
    if (insert_at(items, index, value) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("at-end", std::vector<int>{1, 2}, 2, 3, std::vector<int>{1, 2, 3});
    check("at-front", std::vector<int>{2, 3}, 0, 1, std::vector<int>{1, 2, 3});
    check("in-middle", std::vector<int>{1, 3}, 1, 2, std::vector<int>{1, 2, 3});
    check("empty", std::vector<int>{}, 0, 7, std::vector<int>{7});
    check("single-front", std::vector<int>{5}, 0, 4, std::vector<int>{4, 5});
    check("single-end", std::vector<int>{5}, 1, 6, std::vector<int>{5, 6});
    check("end-of-many", std::vector<int>{1, 2, 3, 4}, 4, 5, std::vector<int>{1, 2, 3, 4, 5});
    check("front-of-many", std::vector<int>{1, 2, 3, 4}, 0, 0, std::vector<int>{0, 1, 2, 3, 4});
    check("duplicates", std::vector<int>{1, 1}, 1, 1, std::vector<int>{1, 1, 1});
    check("limit", std::vector<int>{INT_MAX}, 1, INT_MIN, std::vector<int>{INT_MAX, INT_MIN});
    try {
        const std::vector<int> two{1, 2};
        insert_at(two, 3, 9);
        std::cerr << "case index-past-the-end failed\n";
        failures++;
    } catch (const std::out_of_range &) {
    }
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
