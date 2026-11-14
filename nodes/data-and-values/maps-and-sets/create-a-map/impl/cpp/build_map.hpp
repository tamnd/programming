// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-map/.

#pragma once

#include <string>
#include <utility>
#include <vector>

// build_map puts the pairs in a map, erases the keys in remove, and returns
// the entries in ascending order of the key.
std::vector<std::pair<std::string, int>> build_map(const std::vector<std::pair<std::string, int>> &pairs, const std::vector<std::string> &remove);
