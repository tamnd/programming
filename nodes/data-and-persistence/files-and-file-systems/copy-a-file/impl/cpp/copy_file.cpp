// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-a-file/.

#include "copy_file.hpp"

namespace fs = std::filesystem;

void files::copy_file(const fs::path& src, const fs::path& dst, std::error_code& ec) {
    // std::filesystem::copy_file reports an error for these cases too, but
    // the error code is not the same in each library, so the code checks
    // them first.
    fs::file_status s = fs::status(src, ec);
    if (ec) return;
    if (fs::is_directory(s)) {
        ec = std::make_error_code(std::errc::is_a_directory);
        return;
    }
    fs::file_status d = fs::status(dst, ec);
    ec.clear();
    if (fs::is_directory(d)) {
        ec = std::make_error_code(std::errc::is_a_directory);
        return;
    }
    if (fs::exists(d) && fs::equivalent(src, dst, ec)) {
        ec = std::make_error_code(std::errc::invalid_argument);
        return;
    }
    if (ec) return;
    fs::copy_file(src, dst, fs::copy_options::overwrite_existing, ec);
    if (ec) return;
    // When dst is replaced, the libraries do not all copy the bits.
    fs::permissions(dst, s.permissions(), fs::perm_options::replace, ec);
}
