// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rotate-a-list/.

#pragma once

#include <cstddef>
#include <span>
#include <vector>

// rotate_left returns a new vector with the first k items moved to the end.
std::vector<int> rotate_left(std::span<const int> items, std::size_t k);
