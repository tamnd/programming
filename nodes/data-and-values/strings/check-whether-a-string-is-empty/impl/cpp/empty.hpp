// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-empty/.

#ifndef EMPTY_HPP
#define EMPTY_HPP

#include <string_view>

// is_empty reports whether s has no characters.
bool is_empty(std::string_view s);

// is_blank reports whether each byte of s is ASCII white space.
bool is_blank(std::string_view s);

#endif
