// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-largest-element/.

#pragma once

#include <span>

// largest returns the largest item. It throws std::invalid_argument if items
// is empty.
int largest(std::span<const int> items);
