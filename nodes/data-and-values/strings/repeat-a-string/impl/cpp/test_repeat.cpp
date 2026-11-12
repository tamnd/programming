// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/repeat-a-string/.

#include "repeat.hpp"

#include <iostream>
#include <string>
#include <string_view>

static int failures = 0;

static void check(std::string_view name, const std::string &got, std::string_view want) {
    if (got != want) {
        std::cerr << "case " << name << ": got " << got << ", want " << want << "\n";
        failures++;
    }
}

// The case many has the 200 chars of 100 copies of ab in one literal.
int main() {
    check("word", repeat("ab", 3), "ababab");
    check("one", repeat("abc", 1), "abc");
    check("dash", repeat("-", 5), "-----");
    check("zero", repeat("abc", 0), "");
    check("empty", repeat("", 5), "");
    check("empty-zero", repeat("", 0), "");
    check("many", repeat("ab", 100), "abababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababab");
    check("astral", repeat("\xf0\x9f\x99\x82", 3), "\xf0\x9f\x99\x82\xf0\x9f\x99\x82\xf0\x9f\x99\x82");
    check("combining", repeat("e\xcc\x81", 2), "e\xcc\x81" "e\xcc\x81");
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
