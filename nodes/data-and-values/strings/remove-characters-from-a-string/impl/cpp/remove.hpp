// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-characters-from-a-string/.

#ifndef REMOVE_HPP
#define REMOVE_HPP

#include <string>
#include <string_view>

// remove_chars returns s with each code point of chars removed. s and chars are
// valid UTF-8.
std::string remove_chars(std::string_view s, std::string_view chars);

#endif
