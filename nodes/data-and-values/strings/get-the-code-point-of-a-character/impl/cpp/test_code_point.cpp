// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-code-point-of-a-character/.

#include "code_point.hpp"

#include <iostream>
#include <string_view>

static int failures = 0;

static void check(std::string_view name, uint32_t got, uint32_t want) {
    if (got != want) {
        std::cerr << "case " << name << ": got " << got << ", want " << want << "\n";
        failures++;
    }
}

// U+1F642 is one code point, but it is 2 UTF-16 code units and 4 UTF-8 bytes. U+10FFFF is the largest code point.
int main() {
    check("upper", code_point("A"), 65);
    check("lower", code_point("a"), 97);
    check("digit", code_point("0"), 48);
    check("space", code_point(" "), 32);
    check("tab", code_point("\t"), 9);
    check("tilde", code_point("~"), 126);
    check("two-byte", code_point("\xc3\xa9"), 233);
    check("three-byte", code_point("\xe2\x82\xac"), 8364);
    check("cjk", code_point("\xe6\x97\xa5"), 26085);
    check("combining", code_point("\xcc\x81"), 769);
    check("astral", code_point("\xf0\x9f\x99\x82"), 128578);
    check("max", code_point("\xf4\x8f\xbf\xbf"), 1114111);
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
