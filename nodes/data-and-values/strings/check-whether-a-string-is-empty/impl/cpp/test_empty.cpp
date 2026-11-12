// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-empty/.

#include "empty.hpp"

#include <string_view>
#include <iostream>

static int failures = 0;

static void check(std::string_view name, bool got, bool want) {
    if (got != want) {
        std::cerr << "case " << name << ": got " << got << ", want " << want << "\n";
        failures++;
    }
}

// U+00A0 is NO-BREAK SPACE, U+200B is ZERO WIDTH SPACE, and U+3000 is IDEOGRAPHIC SPACE. None of them is ASCII white space.
int main() {
    check("letter", is_empty("a"), false);
    check("word", is_empty("hello"), false);
    check("empty", is_empty(""), true);
    check("space", is_empty(" "), false);
    check("newline", is_empty("\n"), false);
    check("nbsp", is_empty("\xc2\xa0"), false);
    check("zero-width", is_empty("\xe2\x80\x8b"), false);
    check("combining", is_empty("\xcc\x81"), false);
    check("astral", is_empty("\xf0\x9f\x99\x82"), false);
    check("cjk", is_empty("日"), false);
    check("blank-empty", is_blank(""), true);
    check("blank-space", is_blank(" "), true);
    check("blank-mixed", is_blank(" \t\n\r"), true);
    check("blank-vt-ff", is_blank("\v\f"), true);
    check("blank-letter", is_blank("a"), false);
    check("blank-padded", is_blank("  a  "), false);
    check("blank-nbsp", is_blank("\xc2\xa0"), false);
    check("blank-zero-width", is_blank("\xe2\x80\x8b"), false);
    check("blank-ideographic", is_blank("\xe3\x80\x80"), false);
    check("blank-astral", is_blank(" \xf0\x9f\x99\x82 "), false);
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
