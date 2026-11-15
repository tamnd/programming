// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-while-loop/.

#include "halvings.hpp"

#include <vector>

std::vector<int> halvings(int n) {
    std::vector<int> out;
    while (n > 0) {
        out.push_back(n);
        n /= 2;
    }
    return out;
}
