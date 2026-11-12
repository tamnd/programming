// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-file/.
// since: C++23 (std::expected)

#pragma once

#include <expected>
#include <filesystem>
#include <string_view>

enum class AppendError { invalid_line, cannot_open, write_failed };

// Adds line and one '\n' to the end of the file at path, and makes the file
// if it is missing.
std::expected<void, AppendError> append_line(const std::filesystem::path& path,
                                             std::string_view line);
