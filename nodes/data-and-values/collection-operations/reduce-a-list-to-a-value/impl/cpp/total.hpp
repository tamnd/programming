// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reduce-a-list-to-a-value/.

#pragma once

#include <span>

// total returns the sum of the items. The sum of no items is 0.
int total(std::span<const int> items);
