// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/diff-two-texts/.
// since: C++23 (std::expected)

#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

enum class DiffError { cannot_open, read_failed };

// Splits text at each LF. A final LF does not start a new line.
std::vector<std::string_view> split_lines(std::string_view text);

// Returns the edit script that changes the lines of old_text into the lines of
// new_text. Each line of the result is a space and a line that stays, a - and
// a line of old_text that goes, or a + and a line of new_text that comes, with
// an LF at the end. The script has the smallest number of - and + lines. When
// two scripts are equally short, a - comes before a +.
std::string diff_lines(std::string_view old_text, std::string_view new_text);

// Returns diff_lines of the two files.
std::expected<std::string, DiffError> diff_files(const std::filesystem::path& old_path,
                                                 const std::filesystem::path& new_path);
