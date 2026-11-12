// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/join-a-list-of-strings/.

#include "join.hpp"

#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::string &got, std::string_view want) {
    if (got != want) {
        std::cerr << "case " << name << ": got [" << got << "], want [" << want << "]\n";
        failures++;
    }
}

using V = std::vector<std::string_view>;
using N = std::vector<std::int32_t>;

int main() {
    check("comma", join(V{"a", "b", "c"}, ","), "a,b,c");
    check("comma-space", join(V{"red", "green", "blue"}, ", "), "red, green, blue");
    check("words", join(V{"Hello", "world"}, " "), "Hello world");
    check("empty-list", join(V{}, ","), "");
    check("single", join(V{"only"}, ","), "only");
    check("empty-items", join(V{"", "", ""}, ","), ",,");
    check("empty-middle", join(V{"a", "", "b"}, "-"), "a--b");
    check("empty-sep", join(V{"a", "b", "c"}, ""), "abc");
    check("long-sep", join(V{"x", "y"}, " -> "), "x -> y");
    check("non-ascii", join(V{"naïve", "日本"}, "·"), "naïve·日本");
    check("astral-sep", join(V{"a", "b"}, "\U0001F642"), "a\U0001F642b");
    check("sep-in-item", join(V{"a,b", "c"}, ","), "a,b,c");

    check("numbers", join_numbers(N{1, 2, 3}, ","), "1,2,3");
    check("negative", join_numbers(N{-1, 0, 1}, " "), "-1 0 1");
    check("empty", join_numbers(N{}, ","), "");
    check("single-number", join_numbers(N{42}, ","), "42");
    check("int32-ends", join_numbers(N{INT32_MAX, INT32_MIN}, ";"), "2147483647;-2147483648");

    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
