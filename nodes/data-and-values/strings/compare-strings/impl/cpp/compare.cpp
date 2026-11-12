// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-strings/.

#include "compare.hpp"

int compare(std::string_view a, std::string_view b) {
    // char_traits<char> compares the characters as unsigned char, so the
    // order is the order of the UTF-8 bytes.
    int r = a.compare(b);
    return (r > 0) - (r < 0);
}

bool equals(std::string_view a, std::string_view b) {
    return a == b;
}
