// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-integer-arithmetic/.

#pragma once

#include <array>
#include <cstdint>

// arith returns a + b, a - b, a * b, a / b (rounded toward zero), a % b (with
// the sign of a) and a to the power n. b must not be 0. std::pow works on
// floating-point values, so a loop keeps the power exact.
constexpr std::array<std::int64_t, 6> arith(std::int64_t a, std::int64_t b, int n) {
    std::int64_t power = 1;
    for (int i = 0; i < n; i++) {
        power *= a;
    }
    return {a + b, a - b, a * b, a / b, a % b, power};
}
