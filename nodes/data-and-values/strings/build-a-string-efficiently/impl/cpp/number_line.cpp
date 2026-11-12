// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/build-a-string-efficiently/.

#include "number_line.hpp"

std::string number_line(int n) {
    // std::string grows its buffer when it is full, so each += is amortized
    // O(1), and std::to_string makes the digits of each number.
    std::string r;
    for (int i = 1; i <= n; i++) {
        if (i > 1) {
            r += ',';
        }
        r += std::to_string(i);
    }
    return r;
}
