// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-word-frequencies/.
// since: C++23 (std::expected)

#pragma once

#include <cstddef>
#include <expected>
#include <filesystem>
#include <string>
#include <utility>
#include <vector>

enum class FrequencyError { cannot_open, read_failed };

using WordCounts = std::vector<std::pair<std::string, std::size_t>>;

// Returns the n most frequent words of the file at path, with their counts. A
// word is a run of ASCII letters, changed to lower case. The pairs are sorted
// by count from high to low, then by word.
std::expected<WordCounts, FrequencyError> word_frequencies(const std::filesystem::path& path, std::size_t n);
