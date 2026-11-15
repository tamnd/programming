// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-patch/.
// since: C++23 (std::expected)

#pragma once

#include <cstddef>
#include <expected>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

enum class PatchErrorKind { cannot_open, read_failed, malformed, mismatch };

// The kind of the error, and for malformed and mismatch the number of the line
// of the patch where the error is.
struct PatchError {
    PatchErrorKind kind;
    std::size_t line = 0;
};

// Splits text after each LF. Each line keeps its LF, and a last line with no
// LF is a line too.
std::vector<std::string_view> split_lines(std::string_view text);

// Returns the text that the unified diff patch makes from old_text, with no
// fuzz and no offset. Each hunk must match the lines of old_text at the start
// of its old range.
std::expected<std::string, PatchError> apply_patch(std::string_view old_text, std::string_view patch);

// Returns apply_patch of the two files.
std::expected<std::string, PatchError> apply_patch_file(const std::filesystem::path& path,
                                                        const std::filesystem::path& patch_path);
