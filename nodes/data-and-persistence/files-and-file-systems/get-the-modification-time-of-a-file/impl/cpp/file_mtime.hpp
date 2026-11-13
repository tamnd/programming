// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-modification-time-of-a-file/.

#pragma once

#include <cstdint>
#include <filesystem>
#include <system_error>

// Returns the modification time of the file at path, in whole seconds since
// 1970-01-01T00:00:00Z, with std::filesystem::last_write_time. It follows a
// symbolic link. On an error it sets ec, for example
// std::errc::no_such_file_or_directory, and returns 0.
std::int64_t file_mtime(const std::filesystem::path& path, std::error_code& ec);
