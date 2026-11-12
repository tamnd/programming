// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-the-characters-of-a-string/.

#include "characters.hpp"

namespace {

// seq_len returns the length of the UTF-8 sequence that starts with the byte b.
// A range for loop over a std::string gives single bytes.
std::size_t seq_len(unsigned char b) {
    if (b >= 0xf0) {
        return 4;
    }
    if (b >= 0xe0) {
        return 3;
    }
    if (b >= 0xc0) {
        return 2;
    }
    return 1;
}

}  // namespace

std::vector<std::string> characters(std::string_view s) {
    std::vector<std::string> out;
    std::size_t i = 0;
    while (i < s.size()) {
        std::size_t n = seq_len(static_cast<unsigned char>(s[i]));
        out.emplace_back(s.substr(i, n));
        i += n;
    }
    return out;
}
