// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/partition-a-list/.

#include "partition_odd.hpp"

#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<int> &items, const std::vector<int> &odd,
                  const std::vector<int> &even) {
    auto [got_odd, got_even] = partition_odd(items);
    if (got_odd != odd || got_even != even) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("mixed", std::vector<int>{1, 2, 3, 4, 5}, std::vector<int>{1, 3, 5}, std::vector<int>{2, 4});
    check("tens", std::vector<int>{10, 15, 20, 25}, std::vector<int>{15, 25}, std::vector<int>{10, 20});
    check("empty", std::vector<int>{}, std::vector<int>{}, std::vector<int>{});
    check("all-odd", std::vector<int>{1, 3}, std::vector<int>{1, 3}, std::vector<int>{});
    check("all-even", std::vector<int>{2, 4}, std::vector<int>{}, std::vector<int>{2, 4});
    check("negative", std::vector<int>{-3, -2, -1, 0}, std::vector<int>{-3, -1}, std::vector<int>{-2, 0});
    check("zero", std::vector<int>{0}, std::vector<int>{}, std::vector<int>{0});
    check("order", std::vector<int>{9, 8, 1, 2}, std::vector<int>{9, 1}, std::vector<int>{8, 2});
    check("extremes", std::vector<int>{(-2147483647 - 1), 2147483647}, std::vector<int>{2147483647}, std::vector<int>{(-2147483647 - 1)});
    check("duplicates", std::vector<int>{3, 3, 2, 2}, std::vector<int>{3, 3}, std::vector<int>{2, 2});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
