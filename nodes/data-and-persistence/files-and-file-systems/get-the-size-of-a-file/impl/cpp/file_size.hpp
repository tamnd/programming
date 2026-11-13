// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-size-of-a-file/.

#pragma once

#include <cstdint>
#include <filesystem>
#include <system_error>

// Returns the size in bytes of the regular file at path, with
// std::filesystem::file_size. It follows a symbolic link. On an error it sets
// ec and returns static_cast<std::uintmax_t>(-1): for example
// std::errc::no_such_file_or_directory, or std::errc::is_a_directory for a
// directory. The function is in a namespace, because a call with
// std::filesystem::path arguments also finds std::filesystem::file_size.
namespace files {

std::uintmax_t file_size(const std::filesystem::path& path, std::error_code& ec);

}  // namespace files
