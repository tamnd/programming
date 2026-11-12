// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-length-of-a-string/.

#include "string_length.hpp"

#include <algorithm>

std::array<std::size_t, 3> lengths(std::string_view s) {
    // Each code point has one first byte. A continuation byte is 10xxxxxx.
    auto code_points = static_cast<std::size_t>(
        std::ranges::count_if(s, [](char c) { return (static_cast<unsigned char>(c) & 0xC0) != 0x80; }));
    // A first byte 11110xxx starts a code point above U+FFFF, and UTF-16 needs
    // two code units for it.
    auto four_byte =
        static_cast<std::size_t>(std::ranges::count_if(s, [](char c) { return static_cast<unsigned char>(c) >= 0xF0; }));
    return {s.size(), code_points + four_byte, code_points};
}
