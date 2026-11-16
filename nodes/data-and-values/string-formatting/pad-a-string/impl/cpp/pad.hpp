// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pad-a-string/.

#ifndef PAD_HPP
#define PAD_HPP

#include <cstddef>
#include <string>
#include <string_view>
#include <utility>

// Returns s padded with fill to width, once on the left and once on the
// right. If s has width bytes or more, both results are s.
inline std::pair<std::string, std::string> pad(std::string_view s, std::size_t width, char fill) {
    const std::string extra(width > s.size() ? width - s.size() : 0, fill);
    std::string left = extra;
    left += s;
    std::string right(s);
    right += extra;
    return {left, right};
}

#endif
