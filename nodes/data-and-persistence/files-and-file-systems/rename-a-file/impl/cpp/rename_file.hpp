// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rename-a-file/.

#pragma once

#include <filesystem>
#include <system_error>

// Gives the file at src the name dst, and replaces a file at dst, with one
// rename call. Sets ec on an error, for example std::errc::is_a_directory, or
// std::errc::cross_device_link when the paths are on different file systems.
void rename_file(const std::filesystem::path& src, const std::filesystem::path& dst, std::error_code& ec);
