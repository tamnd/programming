// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/truncate-a-file/.

#include "truncate_file.hpp"

void truncate_file(const std::filesystem::path& path, std::uintmax_t length, std::error_code& ec) {
    // resize_file works as POSIX truncate: it cuts the bytes after length,
    // or adds zero bytes up to length, and it does not make a missing file.
    std::filesystem::resize_file(path, length, ec);
}
