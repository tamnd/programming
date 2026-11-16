// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-tuple/.

#include "min_max.hpp"

#include <algorithm>

std::tuple<std::int32_t, std::int32_t> min_max(const std::vector<std::int32_t> &numbers) {
    std::int32_t lo = numbers[0];
    std::int32_t hi = numbers[0];
    for (std::int32_t n : numbers) {
        lo = std::min(lo, n);
        hi = std::max(hi, n);
    }
    return {lo, hi};
}
