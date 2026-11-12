// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-empty/.

#include "empty.hpp"

bool is_empty(std::string_view s) {
    return s.empty();
}

bool is_blank(std::string_view s) {
    return s.find_first_not_of(" \t\n\v\f\r") == std::string_view::npos;
}
