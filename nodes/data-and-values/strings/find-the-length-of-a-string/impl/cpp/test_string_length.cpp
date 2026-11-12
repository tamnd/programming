// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-length-of-a-string/.

#include <array>
#include <cstddef>
#include <cstdio>
#include <string_view>

#include "string_length.hpp"

int main() {
    using namespace std::string_view_literals;
    struct Case {
        std::string_view name, s;
        std::array<std::size_t, 3> want;
    };
    // The sv literals keep the null character of the nul case.
    const Case cases[] = {
        {"ascii", "hello"sv, {5, 5, 5}},
        {"words", "Hello, world"sv, {12, 12, 12}},
        {"two-byte", "naïve"sv, {6, 5, 5}},
        {"three-byte", "日本語"sv, {9, 3, 3}},
        {"cyrillic", "Привет"sv, {12, 6, 6}},
        {"empty", ""sv, {0, 0, 0}},
        {"single", "a"sv, {1, 1, 1}},
        {"euro-sign", "€"sv, {3, 1, 1}},
        {"astral", "🙂"sv, {4, 2, 1}},
        {"mixed", "a🙂b"sv, {6, 4, 3}},
        {"combining-mark", "e\xcc\x81"sv, {3, 2, 2}},
        {"precomposed", "\xc3\xa9"sv, {2, 1, 1}},
        {"flag", "🇫🇷"sv, {8, 4, 2}},
        {"zwj-family", "👨\xe2\x80\x8d👩\xe2\x80\x8d👧"sv, {18, 8, 5}},
        {"nul", "a\0b"sv, {3, 3, 3}},
    };
    int failures = 0;
    for (const auto& c : cases) {
        if (lengths(c.s) != c.want) {
            std::fprintf(stderr, "case %.*s failed\n", static_cast<int>(c.name.size()), c.name.data());
            ++failures;
        }
    }
    return failures == 0 ? 0 : 1;
}
