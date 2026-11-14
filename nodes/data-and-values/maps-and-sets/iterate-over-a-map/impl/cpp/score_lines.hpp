// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-a-map/.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

// score_lines returns a line key=value for each entry in ascending order of
// the key, then the line total=N with the sum of the values.
std::vector<std::string> score_lines(const std::unordered_map<std::string, int> &scores);
