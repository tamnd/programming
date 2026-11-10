// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/list-files-in-a-directory/.

#ifndef LIST_ENTRIES_HPP
#define LIST_ENTRIES_HPP

#include <filesystem>
#include <string>
#include <vector>

// list_entries returns the relative paths of the entries under root, sorted by
// byte. A directory has a "/" at the end. A symbolic link is listed, but it is
// never followed. It throws std::filesystem::filesystem_error on an error, with
// the code no_such_file_or_directory or not_a_directory for a bad root.
std::vector<std::string> list_entries(const std::filesystem::path& root, bool recursive);

#endif
