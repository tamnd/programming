// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-strings/.

#pragma once

#include <cstdint>
#include <string>
#include <string_view>

// Returns the bytes of a followed by the bytes of b.
std::string concat(std::string_view a, std::string_view b);

// Returns s followed by the decimal form of n.
std::string concat_number(std::string_view s, std::int32_t n);
