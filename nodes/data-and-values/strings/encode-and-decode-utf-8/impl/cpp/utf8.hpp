// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/encode-and-decode-utf-8/.
#pragma once

#include <cstdint>
#include <vector>

// encode_utf8 returns the 1 to 4 UTF-8 bytes of the code point cp. cp must be
// a Unicode scalar value.
std::vector<std::uint8_t> encode_utf8(std::uint32_t cp);

// decode_utf8 returns the code point of the one UTF-8 sequence in b, or -1 if
// b is not exactly one well-formed sequence.
long decode_utf8(const std::vector<std::uint8_t> &b);
