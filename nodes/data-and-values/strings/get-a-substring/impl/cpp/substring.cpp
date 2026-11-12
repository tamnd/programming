// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-substring/.

#include "substring.hpp"

namespace {

// offset returns the byte index of code point k in s, or s.size() for the
// end. A byte of the form 10xxxxxx continues a code point.
std::size_t offset(std::string_view s, std::size_t k) {
    std::size_t n = 0;
    for (std::size_t i = 0; i < s.size(); i++) {
        if ((static_cast<unsigned char>(s[i]) & 0xC0) != 0x80) {
            if (n == k) {
                return i;
            }
            n++;
        }
    }
    return s.size();
}

}  // namespace

std::string substring(std::string_view s, std::size_t start, std::size_t end) {
    // substr takes a start and a count of char elements, which are bytes.
    std::size_t from = offset(s, start);
    return std::string(s.substr(from, offset(s, end) - from));
}
