// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-word-count/.
// since: C++23 (std::expected)

#pragma once

#include <cstdint>
#include <expected>
#include <filesystem>

struct WordCount {
    std::uintmax_t lines = 0;  // the number of LF bytes
    std::uintmax_t words = 0;  // runs of one or more code points that are not White_Space
    std::uintmax_t bytes = 0;
    bool operator==(const WordCount&) const = default;
};

enum class CountError { cannot_open, invalid_utf8, read_failed };

// Counts the lines, words and bytes of the UTF-8 file at path.
std::expected<WordCount, CountError> word_count(const std::filesystem::path& path);
