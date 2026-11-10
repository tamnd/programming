// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/base64-encode-and-decode/.

#include "b64.hpp"

#include <algorithm>
#include <cstddef>
#include <stdexcept>

namespace b64 {

namespace {

constexpr std::string_view standard_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
constexpr std::string_view url_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

std::string_view chars_of(Alphabet alphabet) {
    return alphabet == Alphabet::url ? url_chars : standard_chars;
}

[[noreturn]] void invalid(std::string_view text) {
    throw std::invalid_argument("invalid base64 text: " + std::string(text));
}

}  // namespace

std::string encode(std::span<const std::uint8_t> data, Alphabet alphabet) {
    const std::string_view chars = chars_of(alphabet);
    std::string text;
    text.reserve((data.size() + 2) / 3 * 4);
    for (std::size_t i = 0; i < data.size(); i += 3) {
        const auto chunk = data.subspan(i, std::min<std::size_t>(3, data.size() - i));
        // Put the 1 to 3 bytes in the top 24 bits of v, with zero bits after.
        std::uint32_t v = 0;
        for (std::size_t k = 0; k < 3; k++) {
            v = (v << 8) | (k < chunk.size() ? chunk[k] : 0U);
        }
        for (std::size_t k = 0; k < 4; k++) {
            if (k <= chunk.size()) {
                text += chars[(v >> (18 - 6 * k)) & 63];
            } else if (alphabet == Alphabet::standard) {
                text += '=';
            }
        }
    }
    return text;
}

std::vector<std::uint8_t> decode(std::string_view text, Alphabet alphabet) {
    const std::string_view chars = chars_of(alphabet);
    std::string_view body = text;
    if (alphabet == Alphabet::standard) {
        if (body.size() % 4 != 0) {
            invalid(text);
        }
        // Remove at most two '=' at the end. Any other '=' is not in the
        // alphabet, so the loop below rejects it.
        for (int k = 0; k < 2 && body.ends_with('='); k++) {
            body.remove_suffix(1);
        }
    }
    if (body.size() % 4 == 1) {
        invalid(text);
    }
    std::vector<std::uint8_t> data;
    data.reserve(body.size() / 4 * 3 + 2);
    for (std::size_t i = 0; i < body.size(); i += 4) {
        const std::string_view group = body.substr(i, 4);
        std::uint32_t v = 0;
        for (const char c : group) {
            const std::size_t value = chars.find(c);
            if (value == std::string_view::npos) {
                invalid(text);
            }
            v = (v << 6) | static_cast<std::uint32_t>(value);
        }
        // 4, 3 or 2 characters hold 3, 2 or 1 bytes and 0, 2 or 4 pad bits.
        const std::size_t bytes = group.size() * 6 / 8;
        const std::size_t pad_bits = group.size() * 6 - bytes * 8;
        if ((v & ((1U << pad_bits) - 1)) != 0) {
            invalid(text);
        }
        v >>= pad_bits;
        for (std::size_t k = bytes; k-- > 0;) {
            data.push_back(static_cast<std::uint8_t>(v >> (8 * k)));
        }
    }
    return data;
}

}  // namespace b64
