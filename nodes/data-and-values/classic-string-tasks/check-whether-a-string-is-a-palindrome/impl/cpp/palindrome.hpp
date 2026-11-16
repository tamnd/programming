// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-a-palindrome/.

#ifndef PALINDROME_HPP
#define PALINDROME_HPP

#include <cctype>
#include <cstddef>
#include <string_view>

// Returns true if the ASCII letters and digits of text, in lower case, read
// the same backwards. Two indexes move toward the middle, so the function
// makes no copy of the text.
inline bool is_palindrome(std::string_view text) {
    if (text.size() < 2) {
        return true;
    }
    std::size_t i = 0;
    std::size_t j = text.size() - 1;
    while (i < j) {
        const auto a = static_cast<unsigned char>(text[i]);
        const auto b = static_cast<unsigned char>(text[j]);
        if (std::isalnum(a) == 0) {
            ++i;
        } else if (std::isalnum(b) == 0) {
            --j;
        } else if (std::tolower(a) != std::tolower(b)) {
            return false;
        } else {
            ++i;
            --j;
        }
    }
    return true;
}

#endif
