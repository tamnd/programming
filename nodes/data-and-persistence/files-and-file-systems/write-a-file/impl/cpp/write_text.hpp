// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-file/.

#pragma once

#include <filesystem>
#include <string_view>

enum class WriteMode { Truncate, Append, Atomic };

// Writes text to path, with no change to the bytes. It throws
// std::filesystem::filesystem_error or std::ios_base::failure on an error.
void write_text(const std::filesystem::path &path, std::string_view text, WriteMode mode);
