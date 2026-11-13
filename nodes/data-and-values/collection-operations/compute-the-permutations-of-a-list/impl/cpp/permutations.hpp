// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-permutations-of-a-list/.

#pragma once

#include <cstdint>
#include <vector>

// permutations returns each order of the items, in the lexicographic order of
// the positions.
std::vector<std::vector<std::int32_t>> permutations(const std::vector<std::int32_t> &items);
