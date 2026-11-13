// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-temporary-file/.

#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <system_error>

namespace files {

// Makes a new, empty file in dir whose name is prefix followed by six random
// characters, with the mode 0600, and returns its path. std::filesystem has
// temp_directory_path, but no call that makes a file, so this uses POSIX
// mkstemp, which makes the file with O_CREAT | O_EXCL. On an error it sets ec
// and returns an empty path: std::errc::invalid_argument when prefix has a
// slash, and std::errc::no_such_file_or_directory when dir does not exist.
std::filesystem::path make_temp_file(const std::filesystem::path& dir, std::string_view prefix,
                                     std::error_code& ec);

// Writes data to a new temporary file, reads it back, removes the file, and
// returns the text. On an error it sets ec and returns an empty string.
std::string use_temp_file(const std::filesystem::path& dir, std::string_view prefix,
                          std::string_view data, std::error_code& ec);

}  // namespace files
