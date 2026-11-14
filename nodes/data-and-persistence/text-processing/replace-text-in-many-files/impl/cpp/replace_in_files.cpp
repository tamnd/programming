// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-text-in-many-files/.

#include "replace_in_files.hpp"

#include <fstream>
#include <iterator>
#include <random>
#include <string>
#include <system_error>

namespace fs = std::filesystem;

namespace {

// Writes data to a new file next to path, and renames it over path.
bool write_atomically(const fs::path& path, const std::string& data) {
    std::error_code ec;
    const fs::perms perms = fs::status(path, ec).permissions();
    if (ec) {
        return false;
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
        return false;
    }
    out.write(data.data(), static_cast<std::streamsize>(data.size()));
    out.close();
    if (out) {
        fs::permissions(tmp, perms, ec);
        if (!ec) {
            fs::rename(tmp, path, ec);
            if (!ec) {
                return true;
            }
        }
    }
    fs::remove(tmp, ec);
    return false;
}

}  // namespace

std::expected<std::vector<std::size_t>, ReplaceError> replace_in_files(
    const std::vector<fs::path>& paths, std::string_view old_text, std::string_view new_text) {
    if (old_text.empty()) {
        return std::unexpected(ReplaceError::empty_old);
    }
    std::vector<std::size_t> counts;
    for (const fs::path& path : paths) {
        // Binary mode keeps each CR. In text mode, some systems change CRLF
        // to LF.
        std::ifstream in(path, std::ios::binary);
        if (!in) {
            return std::unexpected(ReplaceError::cannot_open);
        }
        const std::string data{std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
        if (in.bad()) {
            return std::unexpected(ReplaceError::read_failed);
        }
        // find gives the first instance at or after from. The search goes on
        // after the end of each instance.
        std::string out;
        std::size_t count = 0;
        std::size_t from = 0;
        for (std::size_t at; (at = data.find(old_text, from)) != std::string::npos;
             from = at + old_text.size()) {
            out.append(data, from, at - from);
            out.append(new_text);
            count++;
        }
        if (count > 0) {
            out.append(data, from);
            if (!write_atomically(path, out)) {
                return std::unexpected(ReplaceError::write_failed);
            }
        }
        counts.push_back(count);
    }
    return counts;
}
