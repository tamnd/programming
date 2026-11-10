// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-string/.

#include "reverse_string.hpp"

#include <cstddef>

namespace {

// Returns the length of the UTF-8 sequence at the start of s, or 0 if the
// bytes are not valid UTF-8 (RFC 3629). It rejects overlong forms,
// surrogates and values above U+10FFFF.
std::size_t utf8_len(std::string_view s) {
    auto byte = [&](std::size_t i) { return static_cast<unsigned char>(s[i]); };
    const unsigned char c = byte(0);
    std::size_t len = 0;
    unsigned char lo = 0x80, hi = 0xBF;  // the range of the second byte
    if (c < 0x80) {
        return 1;
    } else if (c >= 0xC2 && c <= 0xDF) {
        len = 2;
    } else if (c >= 0xE0 && c <= 0xEF) {
        len = 3;
        if (c == 0xE0) lo = 0xA0;  // no overlong form
        if (c == 0xED) hi = 0x9F;  // no surrogate
    } else if (c >= 0xF0 && c <= 0xF4) {
        len = 4;
        if (c == 0xF0) lo = 0x90;  // no overlong form
        if (c == 0xF4) hi = 0x8F;  // no value above U+10FFFF
    } else {
        return 0;
    }
    if (s.size() < len || byte(1) < lo || byte(1) > hi) return 0;
    for (std::size_t j = 2; j < len; ++j) {
        if ((byte(j) & 0xC0) != 0x80) return 0;
    }
    return len;
}

}  // namespace

std::expected<std::string, ReverseError> reverse_code_points(std::string_view s) {
    std::string out(s.size(), '\0');
    for (std::size_t i = 0; i < s.size();) {
        const std::size_t len = utf8_len(s.substr(i));
        if (len == 0) return std::unexpected(ReverseError::invalid_utf8);
        // The sequence at s[i] goes to the mirror position, with its bytes
        // in the same order.
        s.copy(out.data() + (s.size() - i - len), len, i);
        i += len;
    }
    return out;
}
