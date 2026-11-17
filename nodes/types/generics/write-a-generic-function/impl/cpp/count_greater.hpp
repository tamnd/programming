// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-generic-function/.

#ifndef COUNT_GREATER_HPP
#define COUNT_GREATER_HPP

#include <concepts>
#include <cstddef>
#include <vector>

namespace generic {

// Returns how many items are greater than limit.
template <std::totally_ordered T>
std::size_t count_greater(const std::vector<T>& items, const T& limit) {
    std::size_t n = 0;
    for (const T& item : items) {
        if (item > limit) {
            ++n;
        }
    }
    return n;
}

// The entry for int.
std::size_t count_greater_int(const std::vector<int>& items, int limit);

}  // namespace generic

#endif
