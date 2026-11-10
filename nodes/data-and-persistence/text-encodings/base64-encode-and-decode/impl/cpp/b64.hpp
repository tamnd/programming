// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/base64-encode-and-decode/.

#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace b64 {

// standard is RFC 4648 section 4, padded with '='. url is section 5, with no
// padding.
enum class Alphabet { standard, url };

// Returns the Base64 text of data.
std::string encode(std::span<const std::uint8_t> data, Alphabet alphabet);

// Returns the bytes of text. Throws std::invalid_argument when text is not
// exactly what encode gives for some bytes.
std::vector<std::uint8_t> decode(std::string_view text, Alphabet alphabet);

}  // namespace b64
