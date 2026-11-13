// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/group-items-by-a-key/.

#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

// group_by_length returns one (length, words) pair for each length, in
// ascending order of the length. The words of a pair are in input order.
std::vector<std::pair<std::size_t, std::vector<std::string>>> group_by_length(const std::vector<std::string> &words);
