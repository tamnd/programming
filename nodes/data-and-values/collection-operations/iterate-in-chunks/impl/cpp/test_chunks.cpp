// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-in-chunks/.

#include "chunks.hpp"

#include <cstddef>
#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

using Chunks = std::vector<std::vector<int>>;

static void check(std::string_view name, const std::vector<int> &items, std::size_t size, const Chunks &want) {
    if (chunks(items, size) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("pairs", std::vector<int>{1, 2, 3, 4, 5}, 2, Chunks{{1, 2}, {3, 4}, {5}});
    check("triples", std::vector<int>{1, 2, 3, 4, 5, 6}, 3, Chunks{{1, 2, 3}, {4, 5, 6}});
    check("empty", std::vector<int>{}, 3, Chunks{});
    check("size-one", std::vector<int>{7, 8, 9}, 1, Chunks{{7}, {8}, {9}});
    check("size-larger", std::vector<int>{1, 2}, 5, Chunks{{1, 2}});
    check("size-equal", std::vector<int>{1, 2, 3}, 3, Chunks{{1, 2, 3}});
    check("last-shorter", std::vector<int>{1, 2, 3, 4}, 3, Chunks{{1, 2, 3}, {4}});
    check("negative", std::vector<int>{-1, -2, -3}, 2, Chunks{{-1, -2}, {-3}});
    check("extremes", std::vector<int>{(-2147483647 - 1), 2147483647, 0}, 2, Chunks{{(-2147483647 - 1), 2147483647}, {0}});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
