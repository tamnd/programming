// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-index-of-a-value/.

#pragma once

#include <cstddef>
#include <span>

// index_of returns the first index of value in items, or -1 if it is not there.
std::ptrdiff_t index_of(std::span<const int> items, int value);
