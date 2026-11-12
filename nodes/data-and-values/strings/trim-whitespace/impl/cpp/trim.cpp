// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/trim-whitespace/.

#include "trim.hpp"

namespace {
constexpr std::string_view space = " \t\n\v\f\r";
}

// find_first_not_of returns npos if each char is white space, so the function
// checks it before the call of find_last_not_of.
std::string trim(std::string_view s) {
    std::size_t start = s.find_first_not_of(space);
    if (start == std::string_view::npos) {
        return {};
    }
    std::size_t end = s.find_last_not_of(space);
    return std::string(s.substr(start, end - start + 1));
}

std::string trim_start(std::string_view s) {
    std::size_t start = s.find_first_not_of(space);
    if (start == std::string_view::npos) {
        return {};
    }
    return std::string(s.substr(start));
}
