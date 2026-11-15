// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-a-step/.

#include "count_by.hpp"

#include <vector>

std::vector<int> count_by(int start, int stop, int step) {
    std::vector<int> out;
    // A long long counter: with an int, i + step can overflow, which is undefined.
    for (long long i = start; step > 0 ? i <= stop : i >= stop; i += step) {
        out.push_back(static_cast<int>(i));
    }
    return out;
}
