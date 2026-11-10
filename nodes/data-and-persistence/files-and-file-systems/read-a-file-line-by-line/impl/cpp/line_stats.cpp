// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-file-line-by-line/.
// since: C++23 (std::expected)

#include "line_stats.hpp"

#include <algorithm>
#include <fstream>
#include <optional>
#include <string>
#include <string_view>

namespace {

// Returns the number of code points of s, or nothing when s is not valid
// UTF-8 (RFC 3629): no overlong form, no surrogate, nothing above U+10FFFF.
std::optional<std::size_t> utf8_count(std::string_view s) {
    std::size_t count = 0;
    for (std::size_t i = 0; i < s.size(); ++count) {
        auto c = static_cast<unsigned char>(s[i]);
        std::size_t len = 0;
        unsigned char lo = 0x80, hi = 0xBF;  // the range of the second byte
        if (c < 0x80) {
            ++i;
            continue;
        } else if (c >= 0xC2 && c <= 0xDF) {
            len = 2;
        } else if (c >= 0xE0 && c <= 0xEF) {
            len = 3;
            if (c == 0xE0) lo = 0xA0;
            if (c == 0xED) hi = 0x9F;
        } else if (c >= 0xF0 && c <= 0xF4) {
            len = 4;
            if (c == 0xF0) lo = 0x90;
            if (c == 0xF4) hi = 0x8F;
        } else {
            return std::nullopt;
        }
        if (s.size() - i < len) return std::nullopt;
        auto second = static_cast<unsigned char>(s[i + 1]);
        if (second < lo || second > hi) return std::nullopt;
        for (std::size_t j = 2; j < len; ++j) {
            if ((static_cast<unsigned char>(s[i + j]) & 0xC0) != 0x80) return std::nullopt;
        }
        i += len;
    }
    return count;
}

}  // namespace

std::expected<LineStats, ReadError> line_stats(const std::filesystem::path& path) {
    // Binary mode gives the same bytes on every system; the code removes the '\r' itself.
    std::ifstream in(path, std::ios::binary);
    if (!in) return std::unexpected(ReadError::cannot_open);
    LineStats stats;
    std::string line;
    while (std::getline(in, line)) {  // removes the '\n'
        if (line.ends_with('\r')) line.pop_back();
        auto count = utf8_count(line);
        if (!count) return std::unexpected(ReadError::invalid_utf8);
        ++stats.lines;
        stats.longest = std::max(stats.longest, *count);
    }
    if (in.bad()) return std::unexpected(ReadError::read_failed);
    return stats;
}
