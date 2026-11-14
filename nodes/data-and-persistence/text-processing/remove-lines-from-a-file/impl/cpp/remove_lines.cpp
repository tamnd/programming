// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-lines-from-a-file/.

#include "remove_lines.hpp"

#include <fstream>
#include <random>
#include <string>
#include <system_error>

namespace fs = std::filesystem;

std::expected<int, RemoveError> remove_lines(const fs::path& path, int start, int count) {
    if (start < 1 || count < 0) {
        return std::unexpected(RemoveError::bad_range);
    }
    // Binary mode keeps each CR. In text mode, some systems change CRLF to LF.
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return std::unexpected(RemoveError::cannot_open);
    }
    std::error_code ec;
    const fs::perms perms = fs::status(path, ec).permissions();
    if (ec) {
        return std::unexpected(RemoveError::cannot_open);
    }
    // The new file is next to path, with a random suffix. noreplace (C++23)
    // makes the open fail if a file with that name exists.
    std::random_device random;
    fs::path tmp;
    std::ofstream out;
    for (int tries = 0; tries < 100 && !out.is_open(); tries++) {
        tmp = path;
        tmp += "." + std::to_string(random()) + ".tmp";
        out.open(tmp, std::ios::binary | std::ios::out | std::ios::noreplace);
    }
    if (!out.is_open()) {
        return std::unexpected(RemoveError::write_failed);
    }
    auto fail = [&](RemoveError e) {
        out.close();
        fs::remove(tmp, ec);
        return std::unexpected(e);
    };
    int removed = 0;
    std::string line;
    // getline removes the LF. If it stops at the end of the file, eof() is
    // true, and the line had no LF.
    for (long long n = 1; std::getline(in, line); n++) {
        if (n >= start && n - start < count) {
            removed++;
            continue;
        }
        out << line;
        if (!in.eof()) {
            out << '\n';
        }
    }
    if (in.bad()) {
        return fail(RemoveError::read_failed);
    }
    if (removed == 0) {
        out.close();
        fs::remove(tmp, ec);
        return 0;
    }
    out.close();
    if (!out) {
        return fail(RemoveError::write_failed);
    }
    fs::permissions(tmp, perms, ec);
    if (ec) {
        return fail(RemoveError::write_failed);
    }
    fs::rename(tmp, path, ec);
    if (ec) {
        return fail(RemoveError::write_failed);
    }
    return removed;
}
