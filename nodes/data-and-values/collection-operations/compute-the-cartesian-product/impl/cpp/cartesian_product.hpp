// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-cartesian-product/.

#pragma once

#include <cstdint>
#include <vector>

// cartesian_product returns each tuple that takes one item from each vector,
// in nested-loop order.
std::vector<std::vector<std::int32_t>> cartesian_product(const std::vector<std::vector<std::int32_t>> &lists);
