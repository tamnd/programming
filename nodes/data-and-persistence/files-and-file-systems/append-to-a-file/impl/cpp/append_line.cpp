// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-file/.
// since: C++23 (std::expected)

#include "append_line.hpp"

#include <fstream>
#include <string>

std::expected<void, AppendError> append_line(const std::filesystem::path& path,
                                             std::string_view line) {
    if (line.find_first_of("\r\n") != std::string_view::npos) {
        return std::unexpected(AppendError::invalid_line);
    }
    // std::ios::app seeks to the end before each write. Make the line and its
    // '\n' one string and write it once, so that the stream sends it in one
    // write call when it flushes.
    std::string data{line};
    data += '\n';
    std::ofstream out(path, std::ios::binary | std::ios::app);
    if (!out) return std::unexpected(AppendError::cannot_open);
    out.write(data.data(), static_cast<std::streamsize>(data.size()));
    out.close();  // flushes; close sets failbit if the flush fails
    if (!out) return std::unexpected(AppendError::write_failed);
    return {};
}
