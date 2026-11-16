// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/swap-two-values/.

#ifndef SWAP_HPP
#define SWAP_HPP

#include <cstddef>
#include <utility>
#include <vector>

// Returns a copy of xs with the items at i and j swapped. xs is a copy,
// because the function takes it by value. std::swap exchanges the two
// values with moves.
inline std::vector<long long> swap_items(std::vector<long long> xs, std::size_t i, std::size_t j) {
    std::swap(xs[i], xs[j]);
    return xs;
}

#endif
