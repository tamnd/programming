// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-code-point-of-a-character/.
#pragma once

#include <cstdint>
#include <string_view>

// code_point returns the code point of the UTF-8 sequence at the start of c.
std::uint32_t code_point(std::string_view c);
