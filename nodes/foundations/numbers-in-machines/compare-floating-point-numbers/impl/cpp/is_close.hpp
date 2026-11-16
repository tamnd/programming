// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-floating-point-numbers/.

#pragma once

#include <algorithm>
#include <cmath>

// Tells whether a and b are close: |a - b| <= max(rel_tol * max(|a|, |b|), abs_tol).
// NaN is close to no value, and an infinity is close only to itself.
inline bool is_close(double a, double b, double rel_tol, double abs_tol) {
    if (a == b) {
        return true;  // also an infinity and itself
    }
    if (!std::isfinite(a) || !std::isfinite(b)) {
        return false;  // NaN or an infinity
    }
    const double diff = std::fabs(a - b);
    return diff <= std::max(rel_tol * std::max(std::fabs(a), std::fabs(b)), abs_tol);
}
