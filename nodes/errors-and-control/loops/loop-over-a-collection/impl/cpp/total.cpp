// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-a-collection/.

#include "total.hpp"

#include <vector>

long long total(const std::vector<int> &items) {
    // A long long sum: an int sum can overflow, which is undefined.
    long long s = 0;
    for (int x : items) {
        s += x;
    }
    return s;
}
