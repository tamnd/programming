// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-length-of-an-array/.

#include "length.hpp"

#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<std::string> &items, std::size_t want) {
    if (length(items) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("three", std::vector<std::string>{"a", "b", "c"}, 3);
    check("two-words", std::vector<std::string>{"apple", "banana"}, 2);
    check("empty", std::vector<std::string>{}, 0);
    check("single", std::vector<std::string>{"x"}, 1);
    check("empty-strings", std::vector<std::string>{"", ""}, 2);
    check("non-ascii", std::vector<std::string>{"é", "日本語"}, 2);
    check("duplicates", std::vector<std::string>{"a", "a", "a"}, 3);
    check("ten", std::vector<std::string>{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}, 10);
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
