// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/delete-a-file/.
// since: C++23 (std::expected)

#pragma once

#include <expected>
#include <filesystem>
#include <system_error>

// Deletes the file at path. Gives true if a file was there and false if not.
// A directory gives std::errc::is_a_directory and stays. A symbolic link is
// deleted, not its target. Each other error, for example a permission error,
// gives its error code.
std::expected<bool, std::error_code> delete_file(const std::filesystem::path& path);
