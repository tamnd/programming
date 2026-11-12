// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-a-string-to-bytes/.
#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

// to_bytes returns the bytes of the UTF-8 string s.
std::vector<std::uint8_t> to_bytes(std::string_view s);
