// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-counted-loop/.

#include "count_to.hpp"

#include <vector>

std::vector<int> count_to(int n) {
    std::vector<int> out;
    for (int i = 1; i <= n; i++) {
        out.push_back(i);
    }
    return out;
}
