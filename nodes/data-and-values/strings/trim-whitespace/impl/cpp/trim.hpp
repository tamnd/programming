// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/trim-whitespace/.

#ifndef TRIM_HPP
#define TRIM_HPP

#include <string>
#include <string_view>

// trim returns s with the ASCII white space at the start and at the end
// removed.
std::string trim(std::string_view s);

// trim_start returns s with the white space at the start removed.
std::string trim_start(std::string_view s);

#endif
