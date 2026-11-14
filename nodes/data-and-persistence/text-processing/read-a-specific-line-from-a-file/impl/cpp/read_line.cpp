// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-specific-line-from-a-file/.

#include "read_line.hpp"

#include <fstream>

std::expected<std::optional<std::string>, LineError> read_line(const std::filesystem::path& path,
                                                               int number) {
    // Binary mode keeps each CR. In text mode, some systems change CRLF to LF.
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return std::unexpected(LineError::cannot_open);
    }
    std::string line;
    // getline removes the LF. At the end of the file, it fails only if it
    // read no characters, so a last line with no LF is a line too.
    for (int n = 1; n <= number && std::getline(in, line); n++) {
        if (n == number) {
            return line;
        }
    }
    if (in.bad()) {
        return std::unexpected(LineError::read_failed);
    }
    return std::nullopt;
}
