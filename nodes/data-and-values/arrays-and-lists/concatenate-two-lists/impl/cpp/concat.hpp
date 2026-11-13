// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-two-lists/.

#pragma once

#include <span>
#include <vector>

// concat returns a new vector with the items of a, then the items of b.
std::vector<int> concat(std::span<const int> a, std::span<const int> b);
