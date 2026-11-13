// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-graphemes-of-a-string/.

#include "count_graphemes.hpp"

#include <iostream>
#include <string_view>

static int failures = 0;

static void check(std::string_view name, size_t got, size_t want) {
    if (got != want) {
        std::cerr << "case " << name << ": got " << got << ", want " << want << "\n";
        failures++;
    }
}

// A flag is 2 regional indicators, and U+1F469 U+200D U+1F4BB is 3 code points. Each of them is one grapheme.
int main() {
    check("empty", count_graphemes(""), 0);
    check("ascii", count_graphemes("abc"), 3);
    check("precomposed", count_graphemes("\xc3\xa9"), 1);
    check("emoji", count_graphemes("\xf0\x9f\x99\x82"), 1);
    check("combining", count_graphemes("e\xcc\x81"), 1);
    check("two-marks", count_graphemes("e\xcc\x81\xcc\x82"), 1);
    check("leading-mark", count_graphemes("\xcc\x81" "a"), 2);
    check("crlf", count_graphemes("\r\n"), 1);
    check("lfcr", count_graphemes("\n\r"), 2);
    check("cr-mark", count_graphemes("\r\xcc\x81"), 2);
    check("soft-hyphen", count_graphemes("a\xc2\xad" "b"), 3);
    check("flag", count_graphemes("\xf0\x9f\x87\xab\xf0\x9f\x87\xb7"), 1);
    check("three-indicators", count_graphemes("\xf0\x9f\x87\xab\xf0\x9f\x87\xb7\xf0\x9f\x87\xaa"), 2);
    check("two-flags", count_graphemes("\xf0\x9f\x87\xab\xf0\x9f\x87\xb7\xf0\x9f\x87\xaa\xf0\x9f\x87\xb8"), 2);
    check("zwj-sequence", count_graphemes("\xf0\x9f\x91\xa9\xe2\x80\x8d\xf0\x9f\x92\xbb"), 1);
    check("zwj-after-letter", count_graphemes("a\xe2\x80\x8d\xf0\x9f\x99\x82"), 2);
    check("skin-tone", count_graphemes("\xf0\x9f\x91\x8d\xf0\x9f\x8f\xbd"), 1);
    check("variation-selector", count_graphemes("\xe2\x9d\xa4\xef\xb8\x8f"), 1);
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
