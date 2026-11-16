// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-caesar-or-rot13-cipher/.

#ifndef CAESAR_HPP
#define CAESAR_HPP

#include <algorithm>
#include <string>

// Moves each letter A to Z and a to z shift places along the alphabet, from
// z back to a. Other bytes do not change. A negative shift decodes, and
// caesar(text, 13) is ROT13.
inline std::string caesar(std::string text, long long shift) {
    const int k = static_cast<int>(((shift % 26) + 26) % 26);
    std::ranges::transform(text, text.begin(), [k](char c) {
        if (c >= 'A' && c <= 'Z') {
            return static_cast<char>('A' + (c - 'A' + k) % 26);
        }
        if (c >= 'a' && c <= 'z') {
            return static_cast<char>('a' + (c - 'a' + k) % 26);
        }
        return c;
    });
    return text;
}

#endif
