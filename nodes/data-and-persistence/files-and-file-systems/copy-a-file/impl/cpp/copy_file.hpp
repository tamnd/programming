// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-a-file/.

#pragma once

#include <filesystem>
#include <system_error>

// Copies the bytes and the permission bits of the file at src to dst, and
// replaces a file at dst. Sets ec on an error: std::errc::is_a_directory for a
// directory at src or dst, std::errc::invalid_argument when src and dst are
// the same file, or the error of the copy. The function is in a namespace,
// because a call with path arguments also finds std::filesystem::copy_file.
namespace files {
void copy_file(const std::filesystem::path& src, const std::filesystem::path& dst, std::error_code& ec);
}  // namespace files
