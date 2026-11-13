// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/set-file-permissions/.

#include "set_mode.hpp"

namespace fs = std::filesystem;

namespace files {

unsigned set_mode(const fs::path& path, std::string_view mode, std::error_code& ec) {
    ec.clear();
    unsigned m = 0;
    bool valid = mode.size() >= 3 && mode.size() <= 4;
    for (char c : mode) {
        valid = valid && c >= '0' && c <= '7';
        m = m * 8 + static_cast<unsigned>(c - '0');
    }
    if (!valid) {
        ec = std::make_error_code(std::errc::invalid_argument);
        return 0;
    }
    fs::permissions(path, static_cast<fs::perms>(m), fs::perm_options::replace, ec);
    if (ec) return 0;
    fs::file_status st = fs::status(path, ec);
    if (ec) return 0;
    return static_cast<unsigned>(st.permissions() & fs::perms::mask);
}

}  // namespace files
