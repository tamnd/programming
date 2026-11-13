// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-permutations-of-a-list/.

#include "permutations.hpp"

#include <algorithm>
#include <cstddef>
#include <numeric>

std::vector<std::vector<std::int32_t>> permutations(const std::vector<std::int32_t> &items) {
    // next_permutation orders by value. The indices 0, 1, ... are sorted and
    // all different, so their orders are the orders of the positions, also
    // when items has equal values.
    std::vector<std::size_t> idx(items.size());
    std::iota(idx.begin(), idx.end(), std::size_t{0});
    std::vector<std::vector<std::int32_t>> out;
    do {
        std::vector<std::int32_t> order;
        order.reserve(idx.size());
        for (std::size_t i : idx) {
            order.push_back(items[i]);
        }
        out.push_back(std::move(order));
    } while (std::next_permutation(idx.begin(), idx.end()));
    return out;
}
