// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-word-count/.

#include "word_count.hpp"

#include <fstream>
#include <string>
#include <string_view>

namespace {

// The 25 code points that have the Unicode property White_Space.
bool is_white_space(char32_t c) {
    return (c >= 0x09 && c <= 0x0d) || c == 0x20 || c == 0x85 || c == 0xa0 || c == 0x1680 ||
           (c >= 0x2000 && c <= 0x200a) || c == 0x2028 || c == 0x2029 || c == 0x202f ||
           c == 0x205f || c == 0x3000;
}

// Decodes one UTF-8 sequence at the start of s. It returns the length of the
// sequence and stores the code point in cp, or returns 0 if the bytes are not
// valid UTF-8 (RFC 3629: no overlong forms, no surrogates, nothing above U+10FFFF).
std::size_t decode(std::string_view s, char32_t& cp) {
    const auto b = [&](std::size_t i) { return static_cast<unsigned char>(s[i]); };
    if (b(0) < 0x80) {
        cp = b(0);
        return 1;
    }
    std::size_t len;
    char32_t min;
    if (b(0) >= 0xc2 && b(0) <= 0xdf) {
        len = 2, min = 0x80, cp = b(0) & 0x1fu;
    } else if (b(0) >= 0xe0 && b(0) <= 0xef) {
        len = 3, min = 0x800, cp = b(0) & 0x0fu;
    } else if (b(0) >= 0xf0 && b(0) <= 0xf4) {
        len = 4, min = 0x10000, cp = b(0) & 0x07u;
    } else {
        return 0;
    }
    if (s.size() < len) {
        return 0;
    }
    for (std::size_t i = 1; i < len; ++i) {
        if ((b(i) & 0xc0u) != 0x80) {
            return 0;
        }
        cp = (cp << 6) | (b(i) & 0x3fu);
    }
    if (cp < min || cp > 0x10ffff || (cp >= 0xd800 && cp <= 0xdfff)) {
        return 0;
    }
    return len;
}

}  // namespace

std::expected<WordCount, CountError> word_count(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return std::unexpected(CountError::cannot_open);
    }
    WordCount result;
    std::string line;
    // getline removes the LF. An LF byte is never part of a longer UTF-8
    // sequence, so each line decodes on its own.
    while (std::getline(in, line)) {
        const bool had_lf = !in.eof();
        result.bytes += line.size() + (had_lf ? 1 : 0);
        result.lines += had_lf ? 1 : 0;
        // The LF that ends the line is white space, so a word never goes on to the next line.
        bool in_word = false;
        for (std::string_view s = line; !s.empty();) {
            char32_t cp;
            const std::size_t len = decode(s, cp);
            if (len == 0) {
                return std::unexpected(CountError::invalid_utf8);
            }
            if (is_white_space(cp)) {
                in_word = false;
            } else if (!in_word) {
                in_word = true;
                ++result.words;
            }
            s.remove_prefix(len);
        }
    }
    if (in.bad()) {
        return std::unexpected(CountError::read_failed);
    }
    return result;
}
