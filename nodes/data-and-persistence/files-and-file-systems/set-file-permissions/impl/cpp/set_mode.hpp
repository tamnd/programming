// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/set-file-permissions/.

#pragma once

#include <filesystem>
#include <string_view>
#include <system_error>

namespace files {

// Sets the mode of the file at path to mode, a string of three or four octal
// digits, as for the chmod command, with std::filesystem::permissions, and
// returns the new permission bits (st_mode & 07777). The values of
// std::filesystem::perms are the POSIX bits, so the number casts to perms.
// permissions follows a symbolic link, and the umask does not apply. On an
// error it sets ec and returns 0: std::errc::invalid_argument for a mode that
// is not three or four octal digits, and std::errc::no_such_file_or_directory
// for a missing file.
unsigned set_mode(const std::filesystem::path& path, std::string_view mode, std::error_code& ec);

}  // namespace files
