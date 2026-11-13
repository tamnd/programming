// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/zip-two-lists/.

#pragma once

#include <span>
#include <utility>
#include <vector>

// zip_pairs returns the pairs (a[i], b[i]) up to the end of the shorter span.
std::vector<std::pair<int, int>> zip_pairs(std::span<const int> a, std::span<const int> b);
