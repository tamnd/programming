// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-file-exists/.
// since: C++23 (std::expected)

#pragma once

#include <expected>
#include <filesystem>
#include <string_view>
#include <system_error>

// Tells what is at path: "file", "directory", "other", "missing" or
// "broken-link". On an error other than "no such entry", for example a
// permission error, it gives the error code.
std::expected<std::string_view, std::error_code> file_status(const std::filesystem::path& path);
