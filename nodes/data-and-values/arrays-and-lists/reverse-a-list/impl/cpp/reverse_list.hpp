// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-list/.

#pragma once

#include <span>
#include <vector>

// reverse_list returns a new vector of the items in the opposite order.
std::vector<int> reverse_list(std::span<const int> items);
