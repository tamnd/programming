// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-a-sorted-list/.

#pragma once

#include <span>

// index_of returns the first index of target in the sorted span items, or -1.
long index_of(std::span<const int> items, int target);
