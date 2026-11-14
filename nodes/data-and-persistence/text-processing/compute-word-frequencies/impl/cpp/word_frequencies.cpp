// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-word-frequencies/.

#include "word_frequencies.hpp"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <unordered_map>

std::expected<WordCounts, FrequencyError> word_frequencies(const std::filesystem::path& path, std::size_t n) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return std::unexpected(FrequencyError::cannot_open);
    }
    std::unordered_map<std::string, std::size_t> counts;
    // The current word stays in word across the reads of the stream buffer.
    std::string word;
    for (std::istreambuf_iterator<char> it(in), end; it != end; ++it) {
        const char c = *it;
        if (c >= 'A' && c <= 'Z') {
            word += static_cast<char>(c - 'A' + 'a');
        } else if (c >= 'a' && c <= 'z') {
            word += c;
        } else if (!word.empty()) {
            ++counts[word];  // operator[] inserts a count of 0 for a new word
            word.clear();
        }
    }
    if (in.bad()) {
        return std::unexpected(FrequencyError::read_failed);
    }
    if (!word.empty()) {
        ++counts[word];
    }
    WordCounts ranked(counts.begin(), counts.end());
    // The order of an unordered_map is not fixed, so the word is the second key.
    std::ranges::sort(ranked, [](const auto& a, const auto& b) {
        return a.second != b.second ? a.second > b.second : a.first < b.first;
    });
    ranked.resize(std::min(n, ranked.size()));
    return ranked;
}
