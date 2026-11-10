// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-string/.
// since: C++23 (std::expected)

#pragma once

#include <expected>
#include <string>
#include <string_view>

enum class ReverseError { invalid_utf8 };

// Returns the code points of the UTF-8 bytes s in the opposite order. Each
// code point keeps its bytes in their order.
std::expected<std::string, ReverseError> reverse_code_points(std::string_view s);
