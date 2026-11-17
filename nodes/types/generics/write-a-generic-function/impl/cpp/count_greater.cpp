// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-generic-function/.

#include "count_greater.hpp"

namespace generic {

std::size_t count_greater_int(const std::vector<int>& items, int limit) {
    return count_greater(items, limit);
}

}  // namespace generic
