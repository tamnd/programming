// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-integer-overflow/.

#pragma once

#include <cstdint>
#include <limits>
#include <optional>

// checked_add returns a + b, or nothing when the sum does not fit in
// std::int64_t. A signed overflow is undefined behavior, so the function tests
// the operands before the addition.
constexpr std::optional<std::int64_t> checked_add(std::int64_t a, std::int64_t b) {
    using limits = std::numeric_limits<std::int64_t>;
    if ((b > 0 && a > limits::max() - b) || (b < 0 && a < limits::min() - b)) {
        return std::nullopt;
    }
    return a + b;
}
