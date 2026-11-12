// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-starts-with-a-prefix/.

#ifndef PREFIX_HPP
#define PREFIX_HPP

#include <string_view>

// starts_with reports whether the UTF-8 string s starts with prefix.
bool starts_with(std::string_view s, std::string_view prefix);

// ends_with reports whether the UTF-8 string s ends with suffix.
bool ends_with(std::string_view s, std::string_view suffix);

#endif
