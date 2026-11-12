// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-code-point-of-a-character/.

#include "code_point.hpp"

// code_point decodes one UTF-8 sequence. The first byte gives the length and
// the high bits of the code point. Each continuation byte, 10xxxxxx, gives 6
// more bits. c[0] alone would be only the first byte.
std::uint32_t code_point(std::string_view c) {
    auto b = [&](std::size_t i) { return static_cast<std::uint32_t>(static_cast<unsigned char>(c[i])); };
    std::uint32_t first = b(0);
    if (first < 0x80) {
        return first;
    }
    std::size_t n = first < 0xe0 ? 2 : first < 0xf0 ? 3 : 4;
    std::uint32_t cp = first & (0x7fu >> n);
    for (std::size_t i = 1; i < n; i++) {
        cp = (cp << 6) | (b(i) & 0x3f);
    }
    return cp;
}
