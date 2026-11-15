// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-a-step/.

#pragma once

#include <vector>

// count_by returns start, start + step, ... while the value has not passed
// stop. step must not be 0.
std::vector<int> count_by(int start, int stop, int step);
