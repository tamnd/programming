// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/truncate-a-file/.

#pragma once

#include <cstdint>
#include <filesystem>
#include <system_error>

// Sets the size of the existing file at path to length bytes, with
// std::filesystem::resize_file. Sets ec on an error, for example
// std::errc::no_such_file_or_directory or std::errc::is_a_directory. length
// is unsigned, so it cannot be under 0.
void truncate_file(const std::filesystem::path& path, std::uintmax_t length, std::error_code& ec);
