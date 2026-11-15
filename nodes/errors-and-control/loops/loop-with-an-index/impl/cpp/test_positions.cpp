// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-an-index/.

#include "positions.hpp"

#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<std::string> &items, const std::string &target,
                  const std::vector<std::size_t> &want) {
    if (positions(items, target) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("two-matches", {"a", "b", "a", "c"}, "a", {0, 2});
    check("words", {"red", "green", "blue", "green"}, "green", {1, 3});
    check("empty", {}, "a", {});
    check("none", {"a", "b"}, "z", {});
    check("all", {"x", "x", "x"}, "x", {0, 1, 2});
    check("last", {"a", "b", "c"}, "c", {2});
    check("case", {"A", "a"}, "a", {1});
    check("empty-string", {"", "a", ""}, "", {0, 2});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
