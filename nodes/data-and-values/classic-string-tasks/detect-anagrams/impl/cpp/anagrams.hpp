// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-anagrams/.

#ifndef ANAGRAMS_HPP
#define ANAGRAMS_HPP

#include <array>
#include <cctype>
#include <string_view>

// Returns the count of each ASCII letter of text, with upper case as lower case.
inline std::array<int, 26> letter_counts(std::string_view text) {
    std::array<int, 26> counts{};
    for (const char ch : text) {
        const auto c = static_cast<unsigned char>(ch);
        if (std::isalpha(c) != 0) {
            ++counts[static_cast<std::size_t>(std::tolower(c) - 'a')];
        }
    }
    return counts;
}

// Returns true if a and b hold each letter from a to z the same number of
// times. Case, spaces, punctuation and digits do not count.
inline bool are_anagrams(std::string_view a, std::string_view b) {
    return letter_counts(a) == letter_counts(b);
}

#endif
