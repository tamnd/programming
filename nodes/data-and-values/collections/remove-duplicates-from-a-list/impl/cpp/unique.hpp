// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

#pragma once

#include <string>
#include <type_traits>
#include <unordered_set>
#include <vector>

namespace dedupe {

struct User {
    std::string name;
    std::string email;  // ASCII
    bool operator==(const User&) const = default;
};

// Returns the first item for each key, in input order.
template <class T, class Key>
std::vector<T> unique_by(const std::vector<T>& items, Key key) {
    std::unordered_set<std::decay_t<std::invoke_result_t<Key&, const T&>>> seen;
    std::vector<T> out;
    for (const T& item : items) {
        // insert(...).second is true only if the set did not have the key.
        if (seen.insert(key(item)).second) {
            out.push_back(item);
        }
    }
    return out;
}

// Returns the items without later copies. The first copies keep their order.
std::vector<std::string> unique(const std::vector<std::string>& items);

// Returns the first user for each email. Case does not matter in the emails.
std::vector<User> unique_by_email(const std::vector<User>& users);

}  // namespace dedupe
