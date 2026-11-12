// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-for-a-substring/.

#include "search.hpp"

#include <algorithm>

namespace {

// code_points counts the code points in the first n bytes of s. Each byte that
// does not have the form 10xxxxxx starts a code point.
long code_points(std::string_view s, std::size_t n) {
    return static_cast<long>(std::count_if(s.begin(), s.begin() + static_cast<std::ptrdiff_t>(n),
                                           [](char c) { return (static_cast<unsigned char>(c) & 0xC0) != 0x80; }));
}

}  // namespace

long find(std::string_view s, std::string_view sub) {
    // find returns a byte index, so count the code points before it.
    std::size_t i = s.find(sub);
    return i == std::string_view::npos ? -1 : code_points(s, i);
}

long find_last(std::string_view s, std::string_view sub) {
    std::size_t i = s.rfind(sub);
    return i == std::string_view::npos ? -1 : code_points(s, i);
}
