// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-an-index/.

#include "positions.hpp"

#include <cstddef>
#include <string>
#include <vector>

std::vector<std::size_t> positions(const std::vector<std::string> &items, const std::string &target) {
    std::vector<std::size_t> out;
    for (std::size_t i = 0; i < items.size(); i++) {
        if (items[i] == target) {
            out.push_back(i);
        }
    }
    return out;
}
