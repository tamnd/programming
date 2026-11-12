// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-the-characters-of-a-string/.

#include "characters.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, std::string_view s, const std::vector<std::string> &want) {
    if (characters(s) != want) {
        std::cerr << "case " << name << " fails\n";
        failures++;
    }
}

// A character is a code point. U+1F642 is one character, but two UTF-16 code units and four UTF-8 bytes. A combining mark, a ZWJ and each regional indicator is a character of its own.
int main() {
    check("ascii", "abc", {"a", "b", "c"});
    check("word", "hello", {"h", "e", "l", "l", "o"});
    check("space", "a b", {"a", " ", "b"});
    check("empty", "", {});
    check("single", "x", {"x"});
    check("non-ascii", "h\xc3\xa9", {"h", "\xc3\xa9"});
    check("cjk", "\xe6\x97\xa5\xe6\x9c\xac", {"\xe6\x97\xa5", "\xe6\x9c\xac"});
    check("astral", "a\xf0\x9f\x99\x82" "b", {"a", "\xf0\x9f\x99\x82", "b"});
    check("combining", "e\xcc\x81", {"e", "\xcc\x81"});
    check("regional-indicator", "\xf0\x9f\x87\xba\xf0\x9f\x87\xb8", {"\xf0\x9f\x87\xba", "\xf0\x9f\x87\xb8"});
    check("zwj", "\xf0\x9f\x91\xa9\xe2\x80\x8d\xf0\x9f\x92\xbb", {"\xf0\x9f\x91\xa9", "\xe2\x80\x8d", "\xf0\x9f\x92\xbb"});
    check("control", "a\tb\nc", {"a", "\t", "b", "\n", "c"});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
