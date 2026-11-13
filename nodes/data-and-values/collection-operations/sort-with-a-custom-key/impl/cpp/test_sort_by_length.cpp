// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-with-a-custom-key/.

#include "sort_by_length.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

static int failures = 0;

using Words = std::vector<std::string>;

static void check(std::string_view name, const Words &words, const Words &want) {
    if (sort_by_length(words) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("fruits", Words{"pear", "fig", "apple", "kiwi"}, Words{"fig", "kiwi", "pear", "apple"});
    check("three", Words{"bb", "a", "ccc"}, Words{"a", "bb", "ccc"});
    check("empty", Words{}, Words{});
    check("single", Words{"x"}, Words{"x"});
    check("same-length", Words{"dog", "cat", "ant"}, Words{"ant", "cat", "dog"});
    check("duplicates", Words{"b", "a", "b"}, Words{"a", "b", "b"});
    check("sorted", Words{"a", "bb", "ccc"}, Words{"a", "bb", "ccc"});
    check("reverse", Words{"ccc", "bb", "a"}, Words{"a", "bb", "ccc"});
    check("long", Words{"abcdefghij", "abcdefghi", "z"}, Words{"z", "abcdefghi", "abcdefghij"});
    check("prefix", Words{"ab", "a", "abc", "b"}, Words{"a", "b", "ab", "abc"});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
