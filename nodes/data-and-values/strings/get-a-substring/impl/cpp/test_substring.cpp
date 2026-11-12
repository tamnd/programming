// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-substring/.

#include "substring.hpp"

#include <iostream>
#include <string_view>

static int failures = 0;

static void check(std::string_view name, std::string_view s, std::size_t start, std::size_t end, std::string_view want) {
    std::string got = substring(s, start, end);
    if (got != want) {
        std::cerr << "case " << name << ": got [" << got << "], want [" << want << "]\n";
        failures++;
    }
}

int main() {
    check("middle", "Hello, world", 7, 12, "world");
    check("prefix", "Hello, world", 0, 5, "Hello");
    check("one-char", "abc", 1, 2, "b");
    check("empty", "", 0, 0, "");
    check("whole", "abc", 0, 3, "abc");
    check("empty-range", "abc", 1, 1, "");
    check("at-end", "abc", 3, 3, "");
    check("non-ascii", "naïve café", 2, 5, "ïve");
    check("cjk", "日本語テキスト", 3, 7, "テキスト");
    check("astral", "a\U0001F642b\U0001F643c", 1, 4, "\U0001F642b\U0001F643");
    check("astral-only", "\U0001F642\U0001F643", 1, 2, "\U0001F643");
    // U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
    check("combining", "e\u0301t\u00e9", 0, 1, "e");
    check("combining-mark", "e\u0301t\u00e9", 1, 3, "\u0301t");
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
