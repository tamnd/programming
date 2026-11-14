// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-lines-of-a-file/.

#include "count_lines.hpp"

#include <algorithm>
#include <array>
#include <fstream>

std::expected<LineCount, CountError> count_lines(const std::filesystem::path& path) {
    // Binary mode: on Windows, text mode changes CRLF to LF.
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return std::unexpected(CountError::cannot_open);
    }
    std::array<char, 64 * 1024> buf;
    LineCount result;
    char last = '\n';  // an empty file has no line without an LF
    for (;;) {
        in.read(buf.data(), buf.size());
        // At the end of the file, read sets failbit and eofbit, and gcount gives the bytes it read.
        const std::streamsize n = in.gcount();
        if (n > 0) {
            result.newlines += static_cast<std::uintmax_t>(std::count(buf.data(), buf.data() + n, '\n'));
            last = buf[static_cast<std::size_t>(n - 1)];
        }
        if (in.eof()) {
            break;
        }
        if (!in) {
            return std::unexpected(CountError::read_failed);
        }
    }
    if (in.bad()) {
        return std::unexpected(CountError::read_failed);
    }
    result.lines = result.newlines + (last != '\n' ? 1 : 0);
    return result;
}
