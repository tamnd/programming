// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/flatten-a-nested-list/.

#pragma once

#include <cstdint>
#include <vector>

// flatten returns the items of the inner vectors, one level deep, in order.
std::vector<std::int32_t> flatten(const std::vector<std::vector<std::int32_t>> &lists);
