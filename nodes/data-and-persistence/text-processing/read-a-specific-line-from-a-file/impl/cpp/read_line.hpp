// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-specific-line-from-a-file/.
// since: C++23 (std::expected)

#pragma once

#include <expected>
#include <filesystem>
#include <optional>
#include <string>

enum class LineError { cannot_open, read_failed };

// Returns line number of the file at path, or std::nullopt if there is no
// such line. Line 1 is the first line. A line ends at LF, and the LF is not
// part of it. A last line with no LF is a line too. The function reads the
// file only up to the line.
std::expected<std::optional<std::string>, LineError> read_line(const std::filesystem::path& path,
                                                               int number);
