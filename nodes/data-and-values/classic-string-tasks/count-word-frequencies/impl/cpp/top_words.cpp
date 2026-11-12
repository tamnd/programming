// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-word-frequencies/.

#include "top_words.hpp"

#include <algorithm>
#include <unordered_map>

namespace {

// A byte is part of a word if it is an ASCII letter or digit, or if it is not
// ASCII. Each byte of a multibyte UTF-8 sequence is 0x80 or more. The code does
// not call std::isalnum, because its result depends on the locale.
bool is_word_byte(unsigned char c) {
    return c >= 0x80 || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

}  // namespace

std::vector<std::pair<std::string, std::int64_t>> top_words(std::string_view text, std::size_t n) {
    std::unordered_map<std::string, std::int64_t> counts;
    std::string word;
    for (std::size_t i = 0; i <= text.size(); ++i) {
        unsigned char c = i < text.size() ? static_cast<unsigned char>(text[i]) : ' ';
        if (is_word_byte(c)) {
            word += (c >= 'A' && c <= 'Z') ? static_cast<char>(c - 'A' + 'a') : static_cast<char>(c);
        } else if (!word.empty()) {
            ++counts[word];  // operator[] adds a missing key with the value 0
            word.clear();
        }
    }
    // The order of an unordered_map is not specified, so sort by both keys.
    // std::string compares chars as unsigned char, which for UTF-8 is code
    // point order.
    std::vector<std::pair<std::string, std::int64_t>> out(counts.begin(), counts.end());
    std::ranges::sort(out, [](const auto& a, const auto& b) {
        return a.second != b.second ? a.second > b.second : a.first < b.first;
    });
    out.resize(std::min(n, out.size()));
    return out;
}
