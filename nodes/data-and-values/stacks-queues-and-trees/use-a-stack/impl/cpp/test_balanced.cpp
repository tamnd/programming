// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-stack/.

#include "balanced.hpp"

#include <iostream>
#include <string>
#include <string_view>

static int failures = 0;

static void check(std::string_view label, std::string_view text, bool want) {
    if (balanced(text) != want) {
        std::cerr << "case " << label << " failed\n";
        failures++;
    }
}

int main() {
    const std::string deep = std::string(1000, '(') + std::string(1000, ')');
    check("pair", "()", true);
    check("nested", "{[()()]}", true);
    check("code", "f(a[0], {b: 1})", true);
    check("crossed", "([)]", false);
    check("empty", "", true);
    check("no-brackets", "abc", true);
    check("unclosed", "((", false);
    check("extra-close", "())", false);
    check("close-first", ")(", false);
    check("wrong-kind", "(]", false);
    check("deep", deep, true);
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
