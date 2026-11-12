// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/join-a-list-of-strings/.

#include "join.hpp"

#include <cstddef>

std::string join(std::span<const std::string_view> items, std::string_view sep) {
    // C++23 has std::views::join_with, but the loop also works with a C++20
    // library. It adds up the sizes, reserves once and appends each part.
    std::size_t total = 0;
    for (std::size_t i = 0; i < items.size(); i++) {
        total += items[i].size() + (i > 0 ? sep.size() : 0);
    }
    std::string r;
    r.reserve(total);
    for (std::size_t i = 0; i < items.size(); i++) {
        if (i > 0) {
            r += sep;
        }
        r += items[i];
    }
    return r;
}

std::string join_numbers(std::span<const std::int32_t> items, std::string_view sep) {
    std::string r;
    for (std::size_t i = 0; i < items.size(); i++) {
        if (i > 0) {
            r += sep;
        }
        r += std::to_string(items[i]);
    }
    return r;
}
