// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-file-exists/.
// since: C++23 (std::expected)

#include "file_status.hpp"

namespace fs = std::filesystem;

std::expected<std::string_view, std::error_code> file_status(const fs::path& path) {
    std::error_code ec;
    // status follows symbolic links. For a path that does not exist, it gives
    // file_type::not_found and no error; for other errors, file_type::none.
    fs::file_status st = fs::status(path, ec);
    if (st.type() == fs::file_type::not_found) {
        std::error_code lec;
        return fs::is_symlink(fs::symlink_status(path, lec)) ? "broken-link" : "missing";
    }
    if (ec) return std::unexpected(ec);
    if (fs::is_regular_file(st)) return "file";
    if (fs::is_directory(st)) return "directory";
    return "other";
}
