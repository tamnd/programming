// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-text-in-many-files/.

#pragma once

#include <cstddef>
#include <expected>
#include <filesystem>
#include <string_view>
#include <vector>

enum class ReplaceError { empty_old, cannot_open, read_failed, write_failed };

// Replaces each instance of old_text with new_text in each file, and returns
// the number of instances in each file. old_text and new_text are plain text,
// not patterns. Instances do not overlap. A file with no instance is not
// written. A changed file goes to a new file in the same directory, and
// std::filesystem::rename puts it in the place of the old one. The files are
// done in order, and the first error stops the function: the files before
// have the new text, and the files after do not change.
std::expected<std::vector<std::size_t>, ReplaceError> replace_in_files(
    const std::vector<std::filesystem::path>& paths, std::string_view old_text,
    std::string_view new_text);
