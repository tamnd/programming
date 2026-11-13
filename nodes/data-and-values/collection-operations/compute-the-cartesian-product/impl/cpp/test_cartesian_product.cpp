// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-cartesian-product/.

#include "cartesian_product.hpp"

#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

using List = std::vector<std::int32_t>;
using Lists = std::vector<List>;

static void check(std::string_view name, const Lists &lists, const Lists &want) {
    if (cartesian_product(lists) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("two", Lists{{1, 2}, {3, 4}}, Lists{{1, 3}, {1, 4}, {2, 3}, {2, 4}});
    check("three", Lists{{1, 2}, {3}, {4, 5}}, Lists{{1, 3, 4}, {1, 3, 5}, {2, 3, 4}, {2, 3, 5}});
    check("no-lists", Lists{}, Lists{List{}});
    check("one-empty", Lists{{1, 2}, {}}, Lists{});
    check("single", Lists{{7, 8, 9}}, Lists{{7}, {8}, {9}});
    check("duplicates", Lists{{1, 1}, {2}}, Lists{{1, 2}, {1, 2}});
    check("negative", Lists{{-1}, {-2, -3}}, Lists{{-1, -2}, {-1, -3}});
    check("extremes", Lists{{INT32_MIN, INT32_MAX}, {0}}, Lists{{INT32_MIN, 0}, {INT32_MAX, 0}});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
