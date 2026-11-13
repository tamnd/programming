// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-symbolic-link/.

#include "make_symlink.hpp"

namespace fs = std::filesystem;

namespace files {

fs::path make_symlink(const fs::path& target, const fs::path& link, std::error_code& ec) {
    fs::create_symlink(target, link, ec);
    if (ec) return {};
    return fs::read_symlink(link, ec);
}

}  // namespace files
