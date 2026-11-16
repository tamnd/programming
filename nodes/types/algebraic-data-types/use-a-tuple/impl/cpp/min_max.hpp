// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-tuple/.

#pragma once

#include <cstdint>
#include <tuple>
#include <vector>

// min_max returns (smallest, largest) of a vector with at least one number.
std::tuple<std::int32_t, std::int32_t> min_max(const std::vector<std::int32_t> &numbers);
