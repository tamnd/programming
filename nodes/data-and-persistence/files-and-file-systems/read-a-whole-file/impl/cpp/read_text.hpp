// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-whole-file/.
// since: C++23 (std::expected)

#pragma once

#include <expected>
#include <filesystem>
#include <string>

enum class ReadError { not_found, not_a_file, cannot_open, read_failed, invalid_utf8 };

// Reads all of the UTF-8 text file at path. The bytes come back as they are
// in the file, in a std::string, after a check that they are valid UTF-8.
std::expected<std::string, ReadError> read_text(const std::filesystem::path& path);
