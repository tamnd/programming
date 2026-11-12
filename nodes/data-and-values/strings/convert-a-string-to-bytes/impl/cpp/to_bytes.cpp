// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-a-string-to-bytes/.

#include "to_bytes.hpp"

// A std::string holds bytes, so the function copies them as std::uint8_t.
std::vector<std::uint8_t> to_bytes(std::string_view s) {
    return {s.begin(), s.end()};
}
