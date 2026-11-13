// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-combinations-of-a-list/.

#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

// combinations returns each choice of k items, in the lexicographic order of
// the positions.
std::vector<std::vector<std::int32_t>> combinations(const std::vector<std::int32_t> &items, std::size_t k);
