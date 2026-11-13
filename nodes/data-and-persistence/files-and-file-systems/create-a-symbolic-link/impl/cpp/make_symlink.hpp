// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-symbolic-link/.

#pragma once

#include <filesystem>
#include <system_error>

namespace files {

// Makes a symbolic link at link that holds target, with
// std::filesystem::create_symlink, and returns the target that the link
// holds, read back with read_symlink. The target is stored as it is: it need
// not exist, and a relative target is read from the directory of link. For a
// link to a directory, portable code uses create_directory_symlink, because
// Windows needs to know; on POSIX the two calls are the same. On an error it
// sets ec and returns an empty path: std::errc::file_exists when link exists,
// also as a dangling link, and std::errc::no_such_file_or_directory when the
// directory of link does not exist.
std::filesystem::path make_symlink(const std::filesystem::path& target,
                                   const std::filesystem::path& link, std::error_code& ec);

}  // namespace files
