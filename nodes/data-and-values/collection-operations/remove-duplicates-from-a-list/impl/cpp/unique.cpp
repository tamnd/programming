// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

#include "unique.hpp"

#include <algorithm>
#include <string_view>

namespace dedupe {

std::vector<std::string> unique(const std::vector<std::string>& items) {
    // The views point into items, so the set copies no string.
    return unique_by(items, [](const std::string& s) { return std::string_view(s); });
}

std::vector<User> unique_by_email(const std::vector<User>& users) {
    return unique_by(users, [](const User& u) {
        // Not std::tolower: it uses the current C locale.
        std::string key = u.email;
        std::ranges::transform(key, key.begin(), [](unsigned char c) {
            return static_cast<char>(c >= 'A' && c <= 'Z' ? c - 'A' + 'a' : c);
        });
        return key;
    });
}

}  // namespace dedupe
