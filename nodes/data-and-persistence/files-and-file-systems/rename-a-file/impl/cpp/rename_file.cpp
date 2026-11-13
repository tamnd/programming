// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rename-a-file/.

#include "rename_file.hpp"

void rename_file(const std::filesystem::path& src, const std::filesystem::path& dst, std::error_code& ec) {
    // std::filesystem::rename is POSIX rename: one call, atomic, and it does
    // not copy across file systems.
    std::filesystem::rename(src, dst, ec);
}
