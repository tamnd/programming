// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-with-a-custom-key/.

#include "sort_by_length.hpp"

#include <algorithm>
#include <string_view>
#include <utility>

std::vector<std::string> sort_by_length(std::vector<std::string> words) {
    // The projection gives a key of two parts. A pair compares the first part,
    // then the second part.
    std::ranges::sort(words, {}, [](const std::string &w) { return std::pair{w.size(), std::string_view{w}}; });
    return words;
}
