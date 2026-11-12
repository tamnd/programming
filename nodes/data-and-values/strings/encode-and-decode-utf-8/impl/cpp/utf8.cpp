// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/encode-and-decode-utf-8/.

#include "utf8.hpp"

std::vector<std::uint8_t> encode_utf8(std::uint32_t cp) {
    auto u8 = [](std::uint32_t v) { return static_cast<std::uint8_t>(v); };
    if (cp < 0x80) {
        return {u8(cp)};
    }
    if (cp < 0x800) {
        return {u8(0xc0 | (cp >> 6)), u8(0x80 | (cp & 0x3f))};
    }
    if (cp < 0x10000) {
        return {u8(0xe0 | (cp >> 12)), u8(0x80 | ((cp >> 6) & 0x3f)), u8(0x80 | (cp & 0x3f))};
    }
    return {u8(0xf0 | (cp >> 18)), u8(0x80 | ((cp >> 12) & 0x3f)), u8(0x80 | ((cp >> 6) & 0x3f)),
            u8(0x80 | (cp & 0x3f))};
}

// The first byte gives the length and the high bits. Each continuation byte
// must have the form 10xxxxxx. The result must be at least the smallest code
// point of its length, so an overlong form fails, and it must not be a
// surrogate or above 0x10FFFF.
long decode_utf8(const std::vector<std::uint8_t> &b) {
    if (b.empty()) {
        return -1;
    }
    std::uint8_t first = b[0];
    std::size_t len = first < 0x80 ? 1 : first < 0xc0 ? 0 : first < 0xe0 ? 2 : first < 0xf0 ? 3 : first < 0xf8 ? 4 : 0;
    if (len == 0 || b.size() != len) {
        return -1;
    }
    constexpr std::uint32_t mins[] = {0, 0, 0x80, 0x800, 0x10000};
    std::uint32_t cp = len == 1 ? first : first & (0x7fu >> len);
    for (std::size_t i = 1; i < len; i++) {
        if ((b[i] & 0xc0) != 0x80) {
            return -1;
        }
        cp = (cp << 6) | (b[i] & 0x3fu);
    }
    if (cp < mins[len] || cp > 0x10ffff || (cp >= 0xd800 && cp <= 0xdfff)) {
        return -1;
    }
    return static_cast<long>(cp);
}
