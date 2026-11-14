// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-the-lines-of-a-file/.
// since: C++23 (std::expected)

#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <vector>

enum class SortError { cannot_open, read_failed };

// Returns the lines of the UTF-8 file at path, sorted by their bytes, which is
// the order of the code points. A line ends at LF, and the LF is not part of
// it. A last line with no LF is a line too.
std::expected<std::vector<std::string>, SortError> sort_lines(const std::filesystem::path& path);
