// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-several-collections-at-once/.

#pragma once

#include <string>
#include <vector>

// zip_join joins the items at each index, up to the end of the shortest list.
std::vector<std::string> zip_join(const std::vector<std::string> &lower, const std::vector<std::string> &upper,
                                  const std::vector<int> &digits);
