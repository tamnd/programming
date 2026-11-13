// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-size-of-a-file/.

#include "file_size.hpp"

namespace fs = std::filesystem;

std::uintmax_t files::file_size(const fs::path& path, std::error_code& ec) {
    // fs::file_size reports an error for a directory, but the error code is
    // not the same in each library, so the code checks for a directory first.
    if (fs::is_directory(path, ec)) {
        ec = std::make_error_code(std::errc::is_a_directory);
        return static_cast<std::uintmax_t>(-1);
    }
    return fs::file_size(path, ec);
}
