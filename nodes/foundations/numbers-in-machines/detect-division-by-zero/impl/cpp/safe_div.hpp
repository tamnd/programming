// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-division-by-zero/.

#pragma once

#include <cstdint>
#include <limits>
#include <optional>

// safe_div returns a / b rounded toward zero, or nothing when b is 0 or when
// the quotient does not fit in std::int64_t. Both cases are undefined
// behavior, so the function tests them before the division.
constexpr std::optional<std::int64_t> safe_div(std::int64_t a, std::int64_t b) {
    if (b == 0 || (a == std::numeric_limits<std::int64_t>::min() && b == -1)) {
        return std::nullopt;
    }
    return a / b;
}
