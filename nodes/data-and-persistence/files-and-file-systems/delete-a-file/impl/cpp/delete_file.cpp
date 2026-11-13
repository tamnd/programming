// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/delete-a-file/.
// since: C++23 (std::expected)

#include "delete_file.hpp"

namespace fs = std::filesystem;

std::expected<bool, std::error_code> delete_file(const fs::path& path) {
    std::error_code ec;
    // fs::remove also deletes an empty directory, so look at the entry first,
    // with no link followed.
    const fs::file_status st = fs::symlink_status(path, ec);
    if (st.type() == fs::file_type::not_found) return false;
    if (ec) return std::unexpected(ec);
    if (st.type() == fs::file_type::directory) {
        return std::unexpected(std::make_error_code(std::errc::is_a_directory));
    }
    const bool removed = fs::remove(path, ec);  // false if another program deleted it first
    if (ec == std::errc::no_such_file_or_directory || ec == std::errc::not_a_directory) return false;
    if (ec) return std::unexpected(ec);
    return removed;
}
