// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/change-the-case-of-a-string/.

#include "case.hpp"

// std::toupper depends on the locale and is undefined for a negative char, so
// the functions compare each byte with the ASCII letters.
std::string to_upper(std::string_view s) {
    std::string out(s);
    for (char &c : out) {
        if (c >= 'a' && c <= 'z') {
            c = static_cast<char>(c - 'a' + 'A');
        }
    }
    return out;
}

std::string to_lower(std::string_view s) {
    std::string out(s);
    for (char &c : out) {
        if (c >= 'A' && c <= 'Z') {
            c = static_cast<char>(c - 'A' + 'a');
        }
    }
    return out;
}
