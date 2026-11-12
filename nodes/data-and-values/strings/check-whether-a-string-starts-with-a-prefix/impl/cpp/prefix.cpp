// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-starts-with-a-prefix/.

#include "prefix.hpp"

bool starts_with(std::string_view s, std::string_view prefix) {
    return s.starts_with(prefix);
}

bool ends_with(std::string_view s, std::string_view suffix) {
    return s.ends_with(suffix);
}
