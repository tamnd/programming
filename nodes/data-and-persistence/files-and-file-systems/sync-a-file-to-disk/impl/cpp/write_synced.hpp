// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sync-a-file-to-disk/.

#pragma once

#include <filesystem>
#include <string_view>
#include <system_error>

namespace files {

// Syncs the entries of the directory dir, so that a new name in it is on
// disk. On an error it sets ec.
void sync_directory(const std::filesystem::path& dir, std::error_code& ec);

// Writes text to path, syncs the file, closes it, and syncs the directory of
// path. std::ofstream has no call that syncs to the device, so this uses the
// POSIX calls open, write, fsync and close. On an error it sets ec, for example
// to std::errc::no_such_file_or_directory or std::errc::is_a_directory.
void write_synced(const std::filesystem::path& path, std::string_view text, std::error_code& ec);

}  // namespace files
