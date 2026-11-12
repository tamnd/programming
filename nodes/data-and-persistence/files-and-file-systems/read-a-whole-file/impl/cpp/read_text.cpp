// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-whole-file/.
// since: C++23 (std::expected)

#include "read_text.hpp"

#include <fstream>
#include <iterator>
#include <string_view>
#include <system_error>

namespace {

// Checks that s is valid UTF-8 (RFC 3629): no overlong form, no surrogate,
// nothing above U+10FFFF.
bool utf8_valid(std::string_view s) {
    for (std::size_t i = 0; i < s.size();) {
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
            if (c == 0xE0) lo = 0xA0;  // no overlong form
            if (c == 0xED) hi = 0x9F;  // no surrogate
        } else if (c >= 0xF0 && c <= 0xF4) {
            len = 4;
            if (c == 0xF0) lo = 0x90;  // no overlong form
            if (c == 0xF4) hi = 0x8F;  // no value above U+10FFFF
        } else {
            return false;
        }
        if (s.size() - i < len) return false;
        auto second = static_cast<unsigned char>(s[i + 1]);
        if (second < lo || second > hi) return false;
        for (std::size_t j = 2; j < len; ++j) {
            if ((static_cast<unsigned char>(s[i + j]) & 0xC0) != 0x80) return false;
        }
        i += len;
    }
    return true;
}

}  // namespace

std::expected<std::string, ReadError> read_text(const std::filesystem::path& path) {
    std::error_code ec;
    auto status = std::filesystem::status(path, ec);
    if (status.type() == std::filesystem::file_type::not_found) {
        return std::unexpected(ReadError::not_found);
    }
    // An ifstream opens a directory with no error on Linux, so check first.
    if (std::filesystem::is_directory(status)) return std::unexpected(ReadError::not_a_file);

    std::ifstream in(path, std::ios::binary);  // binary: no change to CR LF
    if (!in) return std::unexpected(ReadError::cannot_open);
    std::string text{std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
    if (in.bad()) return std::unexpected(ReadError::read_failed);
    if (!utf8_valid(text)) return std::unexpected(ReadError::invalid_utf8);
    return text;
}
