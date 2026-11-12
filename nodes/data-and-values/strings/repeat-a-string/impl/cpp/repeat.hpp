// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/repeat-a-string/.

#ifndef REPEAT_HPP
#define REPEAT_HPP

#include <cstddef>
#include <string>
#include <string_view>

// repeat returns n copies of s.
std::string repeat(std::string_view s, std::size_t n);

#endif
