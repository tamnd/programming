// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-lines-from-a-file/.

#pragma once

#include <expected>
#include <filesystem>

enum class RemoveError { bad_range, cannot_open, read_failed, write_failed };

// Removes count lines from the file at path, from line start, and returns how
// many it removed. Line 1 is the first line. A line ends at LF, and the LF
// goes with the line. The result is less than count if the file ends first.
// If no line goes, the file is not written. Otherwise the rest of the file
// goes to a new file in the same directory, and std::filesystem::rename puts
// it in the place of the old one. bad_range is the error when start is less
// than 1 or count is less than 0.
std::expected<int, RemoveError> remove_lines(const std::filesystem::path& path, int start,
                                             int count);
