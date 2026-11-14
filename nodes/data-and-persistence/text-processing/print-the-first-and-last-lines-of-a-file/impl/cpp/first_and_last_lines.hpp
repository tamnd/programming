// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-the-first-and-last-lines-of-a-file/.
// since: C++23 (std::expected)

#pragma once

#include <cstddef>
#include <expected>
#include <filesystem>
#include <string>
#include <utility>
#include <vector>

enum class LinesError { cannot_open, read_failed };

// The first lines and the last lines of a file.
using FirstAndLast = std::pair<std::vector<std::string>, std::vector<std::string>>;

// Returns the first count lines and the last count lines of the file at path.
// If the file has fewer than count lines, each vector has all the lines. A
// line ends at LF, and the LF is not part of it. A last line with no LF is a
// line too.
std::expected<FirstAndLast, LinesError> first_and_last_lines(const std::filesystem::path& path,
                                                             std::size_t count);
