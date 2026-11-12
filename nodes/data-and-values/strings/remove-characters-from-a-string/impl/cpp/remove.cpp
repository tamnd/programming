// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-characters-from-a-string/.

#include "remove.hpp"

#include <cstddef>
#include <vector>

namespace {

// seq_len returns the length of the UTF-8 sequence that starts with the byte b.
std::size_t seq_len(unsigned char b) {
    return b >= 0xf0 ? 4 : b >= 0xe0 ? 3 : b >= 0xc0 ? 2 : 1;
}

// split returns the code points of s, each as the view of its bytes.
std::vector<std::string_view> split(std::string_view s) {
    std::vector<std::string_view> out;
    for (std::size_t i = 0; i < s.size();) {
        std::size_t n = seq_len(static_cast<unsigned char>(s[i]));
        out.push_back(s.substr(i, n));
        i += n;
    }
    return out;
}

}  // namespace

// The function compares whole UTF-8 sequences. A compare of single bytes, as
// with find_first_of, would remove a part of a multibyte sequence.
std::string remove_chars(std::string_view s, std::string_view chars) {
    std::vector<std::string_view> remove = split(chars);
    std::string out;
    out.reserve(s.size());
    for (std::string_view c : split(s)) {
        bool found = false;
        for (std::string_view r : remove) {
            if (r == c) {
                found = true;
                break;
            }
        }
        if (!found) {
            out += c;
        }
    }
    return out;
}
