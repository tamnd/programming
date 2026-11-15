// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-an-index/.

#pragma once

#include <cstddef>
#include <string>
#include <vector>

// positions returns the index of each item that equals target.
std::vector<std::size_t> positions(const std::vector<std::string> &items, const std::string &target);
