// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/list-files-in-a-directory/.

#include "list_entries.hpp"

#include <algorithm>
#include <utility>

namespace fs = std::filesystem;

namespace {

void add(const fs::path& root, const fs::directory_entry& entry, std::vector<std::string>& out) {
    std::string rel = entry.path().lexically_relative(root).generic_string();
    // is_directory follows a symbolic link, so test is_symlink first.
    if (!entry.is_symlink() && entry.is_directory()) rel += '/';
    out.push_back(std::move(rel));
}

}  // namespace

std::vector<std::string> list_entries(const fs::path& root, bool recursive) {
    std::vector<std::string> out;
    if (recursive) {
        // The default options do not follow a symbolic link to a directory.
        for (const auto& entry : fs::recursive_directory_iterator(root)) add(root, entry, out);
    } else {
        for (const auto& entry : fs::directory_iterator(root)) add(root, entry, out);
    }
    // The iteration order is unspecified. std::string compares the bytes as
    // unsigned char, and for UTF-8 that is code point order.
    std::ranges::sort(out);
    return out;
}
