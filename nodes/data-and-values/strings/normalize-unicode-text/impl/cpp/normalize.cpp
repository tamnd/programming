// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/normalize-unicode-text/.
#include "normalize.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace {

constexpr std::string_view vowels = "AEIOUaeiou";
constexpr std::array<char32_t, 4> marks = {0x300, 0x301, 0x302, 0x308};

// composites[i][j] is the composite of vowels[i] in upper case and marks[j].
constexpr std::array<std::array<char32_t, 4>, 5> composites = {{
    {0xC0, 0xC1, 0xC2, 0xC4},
    {0xC8, 0xC9, 0xCA, 0xCB},
    {0xCC, 0xCD, 0xCE, 0xCF},
    {0xD2, 0xD3, 0xD4, 0xD6},
    {0xD9, 0xDA, 0xDB, 0xDC},
}};

char32_t composite(std::size_t i, std::size_t j) {
    return i >= 5 ? composites[i - 5][j] + 0x20 : composites[i][j];
}

std::optional<char32_t> compose(char32_t v, char32_t m) {
    for (std::size_t i = 0; i < vowels.size(); i++) {
        for (std::size_t j = 0; j < marks.size(); j++) {
            if (static_cast<char32_t>(vowels[i]) == v && marks[j] == m) {
                return composite(i, j);
            }
        }
    }
    return std::nullopt;
}

std::optional<std::pair<char32_t, char32_t>> decompose(char32_t c) {
    for (std::size_t i = 0; i < vowels.size(); i++) {
        for (std::size_t j = 0; j < marks.size(); j++) {
            if (composite(i, j) == c) {
                return std::pair{static_cast<char32_t>(vowels[i]), marks[j]};
            }
        }
    }
    return std::nullopt;
}

std::vector<char32_t> decode(std::string_view s) {
    std::vector<char32_t> out;
    for (std::size_t i = 0; i < s.size();) {
        auto b0 = static_cast<unsigned char>(s[i]);
        std::size_t n = b0 < 0x80 ? 1 : b0 < 0xE0 ? 2 : b0 < 0xF0 ? 3 : 4;
        char32_t c = n == 1 ? b0 : (b0 & (0x7Fu >> n));
        for (std::size_t j = 1; j < n; j++) {
            c = (c << 6) | (static_cast<unsigned char>(s[i + j]) & 0x3Fu);
        }
        out.push_back(c);
        i += n;
    }
    return out;
}

void encode(std::string &out, char32_t c) {
    if (c < 0x80) {
        out += static_cast<char>(c);
    } else {
        out += static_cast<char>(0xC0 | (c >> 6));
        out += static_cast<char>(0x80 | (c & 0x3F));
    }
}

} // namespace

std::string normalize(std::string_view s, std::string_view form) {
    std::vector<char32_t> out;
    for (char32_t c : decode(s)) {
        if (form == "NFD") {
            if (auto d = decompose(c)) {
                out.push_back(d->first);
                out.push_back(d->second);
                continue;
            }
        } else if (!out.empty()) {
            if (auto p = compose(out.back(), c)) {
                out.back() = *p;
                continue;
            }
        }
        out.push_back(c);
    }
    std::string r;
    for (char32_t c : out) {
        encode(r, c);
    }
    return r;
}
