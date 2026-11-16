// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-separators/.

#ifndef GROUP_HPP
#define GROUP_HPP

#include <cstddef>
#include <cstdint>
#include <string>

// Returns n in decimal with a comma between each group of 3 digits. It
// converts n to text first, so the smallest int64_t does not overflow.
inline std::string group_thousands(std::int64_t n) {
    const std::string s = std::to_string(n);
    const std::size_t start = s[0] == '-' ? 1 : 0;
    const std::size_t len = s.size() - start;
    std::string out = s.substr(0, start);
    for (std::size_t i = 0; i < len; ++i) {
        if (i > 0 && (len - i) % 3 == 0) {
            out += ',';
        }
        out += s[start + i];
    }
    return out;
}

#endif
