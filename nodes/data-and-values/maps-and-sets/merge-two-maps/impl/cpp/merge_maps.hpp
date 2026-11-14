// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/merge-two-maps/.

#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// merge_maps returns the entries of both maps in ascending order of the key.
// For a key in both maps, the value of over wins.
std::vector<std::pair<std::string, int>> merge_maps(const std::unordered_map<std::string, int> &base, const std::unordered_map<std::string, int> &over);
