// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicate-lines/.
// since: C++23 (std::expected)

#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <vector>

enum class DedupError { cannot_open, read_failed };

// Returns the lines of the file at path, with only the first copy of each
// line, in the order of the file. Two lines are equal if their bytes are
// equal. A line ends at LF, and the LF is not part of it. A last line with no
// LF is a line too.
std::expected<std::vector<std::string>, DedupError> remove_duplicate_lines(const std::filesystem::path& path);
