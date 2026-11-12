// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-a-substring/.

#ifndef REPLACE_HPP
#define REPLACE_HPP

#include <string>
#include <string_view>

// replace returns s with each match of old replaced by new_. old must not be
// empty.
std::string replace(std::string_view s, std::string_view old, std::string_view new_);

// replace_first is like replace, but it replaces only the first match.
std::string replace_first(std::string_view s, std::string_view old, std::string_view new_);

#endif
