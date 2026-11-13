// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-graphemes-of-a-string/.
#ifndef COUNT_GRAPHEMES_HPP
#define COUNT_GRAPHEMES_HPP

#include <cstddef>
#include <string_view>

// count_graphemes returns the number of extended grapheme clusters in the
// UTF-8 string s. s.size() counts bytes.
std::size_t count_graphemes(std::string_view s);

#endif
