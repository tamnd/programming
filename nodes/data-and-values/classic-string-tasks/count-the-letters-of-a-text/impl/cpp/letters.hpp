// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-letters-of-a-text/.

#ifndef LETTERS_HPP
#define LETTERS_HPP

#include <cctype>
#include <cstddef>
#include <string_view>
#include <vector>

// Returns the counts of the letters a to z in text, in the order of the
// alphabet. Upper case letters count as lower case letters.
inline std::vector<long long> count_letters(std::string_view text) {
    std::vector<long long> counts(26);
    for (const char ch : text) {
        const auto c = static_cast<unsigned char>(ch);
        if (std::isalpha(c) != 0) {
            ++counts[static_cast<std::size_t>(std::tolower(c) - 'a')];
        }
    }
    return counts;
}

#endif
