// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-strings/.

#ifndef COMPARE_HPP
#define COMPARE_HPP

#include <string_view>

// compare returns -1, 0 or 1 for the code point order of the UTF-8 strings a
// and b.
int compare(std::string_view a, std::string_view b);

// equals reports whether a and b have the same bytes.
bool equals(std::string_view a, std::string_view b);

#endif
