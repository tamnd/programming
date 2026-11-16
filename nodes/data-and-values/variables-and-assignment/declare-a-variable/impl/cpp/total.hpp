// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-variable/.

#ifndef TOTAL_HPP
#define TOTAL_HPP

#include <vector>

// Returns the sum of values. The variable result has a named type, and
// the loop variable uses auto, so the compiler infers its type.
inline long long total(const std::vector<long long>& values) {
    long long result = 0;
    for (const auto value : values) {
        result = result + value;
    }
    return result;
}

#endif
