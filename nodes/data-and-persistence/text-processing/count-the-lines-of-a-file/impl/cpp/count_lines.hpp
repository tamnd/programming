// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-lines-of-a-file/.
// since: C++23 (std::expected)

#pragma once

#include <cstdint>
#include <expected>
#include <filesystem>

struct LineCount {
    std::uintmax_t newlines = 0;  // the number of LF bytes, as wc -l prints it
    std::uintmax_t lines = 0;     // also counts a last line that has no LF
    bool operator==(const LineCount&) const = default;
};

enum class CountError { cannot_open, read_failed };

// Counts the lines of the file at path. It reads the bytes and does not decode them.
std::expected<LineCount, CountError> count_lines(const std::filesystem::path& path);
