// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-set-operations/.

#include "compare_sets.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

static int failures = 0;

using Words = std::vector<std::string>;

static void check(std::string_view name, const Words &a, const Words &b, const SetOps &want) {
    if (compare_sets(a, b) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("colors", Words{"red", "blue", "green"}, Words{"blue", "yellow"}, SetOps{Words{"blue", "green", "red", "yellow"}, Words{"blue"}, Words{"green", "red"}, Words{"green", "red", "yellow"}, false});
    check("subset", Words{"b"}, Words{"a", "b", "c"}, SetOps{Words{"a", "b", "c"}, Words{"b"}, Words{}, Words{"a", "c"}, true});
    check("both-empty", Words{}, Words{}, SetOps{Words{}, Words{}, Words{}, Words{}, true});
    check("empty-a", Words{}, Words{"x"}, SetOps{Words{"x"}, Words{}, Words{}, Words{"x"}, true});
    check("empty-b", Words{"x"}, Words{}, SetOps{Words{"x"}, Words{}, Words{"x"}, Words{"x"}, false});
    check("equal", Words{"a", "b"}, Words{"b", "a"}, SetOps{Words{"a", "b"}, Words{"a", "b"}, Words{}, Words{}, true});
    check("disjoint", Words{"a"}, Words{"b"}, SetOps{Words{"a", "b"}, Words{}, Words{"a"}, Words{"a", "b"}, false});
    check("duplicates", Words{"a", "a", "b"}, Words{"b", "b"}, SetOps{Words{"a", "b"}, Words{"b"}, Words{"a"}, Words{"a"}, false});
    check("case", Words{"a"}, Words{"A"}, SetOps{Words{"A", "a"}, Words{}, Words{"a"}, Words{"A", "a"}, false});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
