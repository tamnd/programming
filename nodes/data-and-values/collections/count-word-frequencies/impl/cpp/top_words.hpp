// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-word-frequencies/.

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// Returns the n most frequent words of the UTF-8 text with their counts, by
// count from high to low, then by word in byte order, which for UTF-8 is code
// point order.
std::vector<std::pair<std::string, std::int64_t>> top_words(std::string_view text, std::size_t n);
