// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-lines-that-match-a-pattern/.
// since: C++23 (std::expected)

#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <vector>

enum class FilterError { cannot_open, read_failed };

// Returns the lines of the file at path that match pattern, a POSIX extended
// regular expression. A line matches if the pattern matches some part of it.
// A line ends at LF, and the LF is not part of it. A last line with no LF is a
// line too. A pattern that is not valid throws std::regex_error.
std::expected<std::vector<std::string>, FilterError> filter_lines(const std::filesystem::path& path,
                                                                  const std::string& pattern);
