// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-a-substring/.

#include "replace.hpp"

namespace {

// replace_n replaces at most limit matches. It appends the text between the
// matches and the new text to the result, and it searches again after the end
// of each match.
std::string replace_n(std::string_view s, std::string_view old, std::string_view new_, std::size_t limit) {
    std::string out;
    std::size_t from = 0;
    for (std::size_t n = 0; n < limit; n++) {
        std::size_t i = s.find(old, from);
        if (i == std::string_view::npos) {
            break;
        }
        out += s.substr(from, i - from);
        out += new_;
        from = i + old.size();
    }
    out += s.substr(from);
    return out;
}

}  // namespace

std::string replace(std::string_view s, std::string_view old, std::string_view new_) {
    return replace_n(s, old, new_, std::string_view::npos);
}

std::string replace_first(std::string_view s, std::string_view old, std::string_view new_) {
    return replace_n(s, old, new_, 1);
}
