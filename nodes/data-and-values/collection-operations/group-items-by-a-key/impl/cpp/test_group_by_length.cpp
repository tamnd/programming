// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/group-items-by-a-key/.

#include "group_by_length.hpp"

#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

static int failures = 0;

using Groups = std::vector<std::pair<std::size_t, std::vector<std::string>>>;

static void check(std::string_view name, const std::vector<std::string> &words, const Groups &want) {
    if (group_by_length(words) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("fruits", std::vector<std::string>{"apple", "fig", "kiwi", "pear", "plum", "date"}, Groups{{3, {"fig"}}, {4, {"kiwi", "pear", "plum", "date"}}, {5, {"apple"}}});
    check("letters", std::vector<std::string>{"a", "bb", "c"}, Groups{{1, {"a", "c"}}, {2, {"bb"}}});
    check("empty", std::vector<std::string>{}, Groups{});
    check("single", std::vector<std::string>{"x"}, Groups{{1, {"x"}}});
    check("same-length", std::vector<std::string>{"cat", "dog", "cow"}, Groups{{3, {"cat", "dog", "cow"}}});
    check("order", std::vector<std::string>{"ccc", "a", "bb"}, Groups{{1, {"a"}}, {2, {"bb"}}, {3, {"ccc"}}});
    check("duplicates", std::vector<std::string>{"ab", "ab", "c"}, Groups{{1, {"c"}}, {2, {"ab", "ab"}}});
    check("long", std::vector<std::string>{"abcdefghij", "abcdefghi", "a"}, Groups{{1, {"a"}}, {9, {"abcdefghi"}}, {10, {"abcdefghij"}}});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
