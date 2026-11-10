// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-file-line-by-line/.
// since: C++23 (std::expected)

#pragma once

#include <cstddef>
#include <expected>
#include <filesystem>

struct LineStats {
    std::size_t lines = 0;
    std::size_t longest = 0;  // in code points
    bool operator==(const LineStats&) const = default;
};

enum class ReadError { cannot_open, invalid_utf8, read_failed };

// Reads the UTF-8 text file at path line by line.
std::expected<LineStats, ReadError> line_stats(const std::filesystem::path& path);
