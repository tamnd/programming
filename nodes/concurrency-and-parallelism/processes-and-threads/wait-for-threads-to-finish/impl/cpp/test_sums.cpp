// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wait-for-threads-to-finish/.

#include "sums.hpp"

#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<std::vector<std::int64_t>>& chunks,
                  const std::vector<std::int64_t>& want) {
    if (sums::parallel_sums(chunks) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    const std::int64_t g = 1000000000;
    check("two", {{1, 2, 3}, {4, 5}}, {6, 9});
    check("three", {{10}, {20, 30}, {-5, 5}}, {10, 50, 0});
    check("empty", {}, {});
    check("empty-chunk", {{}, {7}}, {0, 7});
    check("single", {{42}}, {42});
    check("negative", {{-1, -2}, {-3}}, {-3, -3});
    check("limit", {{g, g, g}, {-g, -g, -g}}, {3 * g, -3 * g});

    std::vector<std::vector<std::int64_t>> many;
    std::vector<std::int64_t> want;
    for (std::int64_t i = 0; i < 32; i++) {
        many.push_back({i, i, i});
        want.push_back(3 * i);
    }
    check("many", many, want);

    if (failures == 0) {
        std::cout << "all tests passed\n";
    }
    return failures == 0 ? 0 : 1;
}
