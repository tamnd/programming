// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/change-the-case-of-a-string/.

#ifndef CASE_HPP
#define CASE_HPP

#include <string>
#include <string_view>

// to_upper returns s with a to z changed to A to Z. The other bytes of the
// UTF-8 string do not change.
std::string to_upper(std::string_view s);

// to_lower returns s with A to Z changed to a to z.
std::string to_lower(std::string_view s);

#endif
