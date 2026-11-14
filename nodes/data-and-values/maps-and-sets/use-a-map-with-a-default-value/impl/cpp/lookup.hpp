// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

// lookup returns the value of each key in table, or fallback for a key that
// is not in table.
std::vector<int> lookup(const std::unordered_map<std::string, int> &table, const std::vector<std::string> &keys, int fallback);
