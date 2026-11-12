// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-length-of-a-string/.

#pragma once

#include <array>
#include <cstddef>
#include <string_view>

// Returns three lengths of the valid UTF-8 string s: the number of bytes, the
// number of UTF-16 code units and the number of code points.
std::array<std::size_t, 3> lengths(std::string_view s);
