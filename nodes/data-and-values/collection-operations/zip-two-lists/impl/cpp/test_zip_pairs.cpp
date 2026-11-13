// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/zip-two-lists/.

#include "zip_pairs.hpp"

#include <iostream>
#include <string_view>
#include <utility>
#include <vector>

static int failures = 0;

using Pairs = std::vector<std::pair<int, int>>;

static void check(std::string_view name, const std::vector<int> &a, const std::vector<int> &b, const Pairs &want) {
    if (zip_pairs(a, b) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("same-length", std::vector<int>{1, 2, 3}, std::vector<int>{4, 5, 6}, Pairs{{1, 4}, {2, 5}, {3, 6}});
    check("tens", std::vector<int>{10, 20}, std::vector<int>{30, 40}, Pairs{{10, 30}, {20, 40}});
    check("empty", std::vector<int>{}, std::vector<int>{}, Pairs{});
    check("first-empty", std::vector<int>{}, std::vector<int>{1, 2}, Pairs{});
    check("second-empty", std::vector<int>{1, 2}, std::vector<int>{}, Pairs{});
    check("first-shorter", std::vector<int>{1}, std::vector<int>{7, 8}, Pairs{{1, 7}});
    check("second-shorter", std::vector<int>{1, 2, 3}, std::vector<int>{9}, Pairs{{1, 9}});
    check("negative", std::vector<int>{-1, -2}, std::vector<int>{3, -4}, Pairs{{-1, 3}, {-2, -4}});
    check("extremes", std::vector<int>{(-2147483647 - 1), 2147483647}, std::vector<int>{2147483647, (-2147483647 - 1)}, Pairs{{(-2147483647 - 1), 2147483647}, {2147483647, (-2147483647 - 1)}});
    check("duplicates", std::vector<int>{5, 5}, std::vector<int>{5, 5}, Pairs{{5, 5}, {5, 5}});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
