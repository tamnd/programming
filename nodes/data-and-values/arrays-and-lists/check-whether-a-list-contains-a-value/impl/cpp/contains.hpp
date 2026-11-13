// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

#pragma once

#include <span>

// contains returns true if some item of items is equal to value.
bool contains(std::span<const int> items, int value);
