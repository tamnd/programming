// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/repeat-a-string/.

#include "repeat.hpp"

// std::string has a constructor for n copies of one char, but not for n copies
// of a string, so the function reserves the length and appends s n times.
std::string repeat(std::string_view s, std::size_t n) {
    std::string out;
    out.reserve(s.size() * n);
    for (std::size_t i = 0; i < n; i++) {
        out += s;
    }
    return out;
}
