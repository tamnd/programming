// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-set/.

#pragma once

#include <string>
#include <vector>

// toggle adds each word that is not in the set, and removes each word that is
// in it. It returns the words of the set in ascending order.
std::vector<std::string> toggle(const std::vector<std::string> &words);
