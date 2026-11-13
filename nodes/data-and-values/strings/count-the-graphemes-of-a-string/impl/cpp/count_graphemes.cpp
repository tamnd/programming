// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-graphemes-of-a-string/.
#include "count_graphemes.hpp"

#include <cstdint>

namespace {

enum class kind { other, cr, lf, control, extend, zwj, ri, pict };

// kind_of returns the Grapheme_Cluster_Break kind of c, or pict for
// Extended_Pictographic.
kind kind_of(std::uint32_t c) {
    if (c == 0x0D) {
        return kind::cr;
    }
    if (c == 0x0A) {
        return kind::lf;
    }
    if (c < 0x20 || (c >= 0x7F && c <= 0x9F) || c == 0xAD) {
        return kind::control;
    }
    if ((c >= 0x300 && c <= 0x36F) || (c >= 0xFE00 && c <= 0xFE0F) || (c >= 0x1F3FB && c <= 0x1F3FF)) {
        return kind::extend;
    }
    if (c == 0x200D) {
        return kind::zwj;
    }
    if (c >= 0x1F1E6 && c <= 0x1F1FF) {
        return kind::ri;
    }
    if (c == 0xA9 || c == 0xAE || c == 0x2764 || (c >= 0x1F400 && c <= 0x1F4FD) || (c >= 0x1F600 && c <= 0x1F64F)) {
        return kind::pict;
    }
    return kind::other;
}

// decode reads the UTF-8 sequence at s[i], and moves i past it.
std::uint32_t decode(std::string_view s, std::size_t &i) {
    auto b0 = static_cast<unsigned char>(s[i]);
    std::size_t n = b0 < 0x80 ? 1 : b0 < 0xE0 ? 2 : b0 < 0xF0 ? 3 : 4;
    std::uint32_t c = n == 1 ? b0 : (b0 & (0x7Fu >> n));
    for (std::size_t j = 1; j < n; j++) {
        c = (c << 6) | (static_cast<unsigned char>(s[i + j]) & 0x3Fu);
    }
    i += n;
    return c;
}

bool is_break_kind(kind k) {
    return k == kind::cr || k == kind::lf || k == kind::control;
}

} // namespace

std::size_t count_graphemes(std::string_view s) {
    std::size_t n = 0;
    std::size_t ri_run = 0;
    bool have_prev = false;
    bool pict = false;
    bool zwj_after_pict = false;
    kind prev = kind::other;
    for (std::size_t i = 0; i < s.size();) {
        kind k = kind_of(decode(s, i));
        bool join = false;
        if (!have_prev) {
            join = false;
        } else if (prev == kind::cr && k == kind::lf) {
            join = true; // GB3
        } else if (is_break_kind(prev) || is_break_kind(k)) {
            join = false; // GB4, GB5
        } else if (k == kind::extend || k == kind::zwj) {
            join = true; // GB9
        } else if (k == kind::pict && zwj_after_pict) {
            join = true; // GB11
        } else if (k == kind::ri && ri_run % 2 == 1) {
            join = true; // GB12, GB13
        }
        if (!join) {
            n++;
        }
        zwj_after_pict = k == kind::zwj && pict;
        pict = k == kind::pict || (k == kind::extend && pict);
        ri_run = k == kind::ri ? ri_run + 1 : 0;
        prev = k;
        have_prev = true;
    }
    return n;
}
