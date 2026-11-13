// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/slice-a-list/.

#include "slice_list.hpp"

#include <climits>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<int> &items, std::size_t start, std::size_t stop, const std::vector<int> &want) {
    if (slice_list(items, start, stop) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

static bool throws(std::size_t start, std::size_t stop) {
    const std::vector<int> three{1, 2, 3};
    try {
        slice_list(three, start, stop);
    } catch (const std::out_of_range &) {
        return true;
    }
    return false;
}

int main() {
    check("middle", std::vector<int>{1, 2, 3, 4, 5}, 1, 3, std::vector<int>{2, 3});
    check("prefix", std::vector<int>{1, 2, 3}, 0, 2, std::vector<int>{1, 2});
    check("suffix", std::vector<int>{1, 2, 3}, 1, 3, std::vector<int>{2, 3});
    check("empty-list", std::vector<int>{}, 0, 0, std::vector<int>{});
    check("empty-slice", std::vector<int>{1, 2, 3}, 2, 2, std::vector<int>{});
    check("whole", std::vector<int>{1, 2, 3}, 0, 3, std::vector<int>{1, 2, 3});
    check("single", std::vector<int>{7}, 0, 1, std::vector<int>{7});
    check("last-one", std::vector<int>{1, 2, 3}, 2, 3, std::vector<int>{3});
    check("at-end", std::vector<int>{1, 2, 3}, 3, 3, std::vector<int>{});
    check("limit", std::vector<int>{INT_MAX, INT_MIN, 0}, 0, 2, std::vector<int>{INT_MAX, INT_MIN});
    if (!throws(0, 4) || !throws(2, 1)) {
        std::cerr << "case bad-range failed\n";
        failures++;
    }
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
