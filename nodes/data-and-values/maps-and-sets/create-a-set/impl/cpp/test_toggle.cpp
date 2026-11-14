// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-set/.

#include "toggle.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

static int failures = 0;

using Words = std::vector<std::string>;

static void check(std::string_view name, const Words &words, const Words &want) {
    if (toggle(words) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("colors", Words{"red", "blue", "red", "green"}, Words{"blue", "green"});
    check("fresh", Words{"b", "a"}, Words{"a", "b"});
    check("empty", Words{}, Words{});
    check("two-times", Words{"a", "a"}, Words{});
    check("three-times", Words{"a", "a", "a"}, Words{"a"});
    check("case", Words{"a", "A"}, Words{"A", "a"});
    check("prefix", Words{"ab", "a"}, Words{"a", "ab"});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
