// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-higher-order-function/.

#ifndef REPEAT_HPP
#define REPEAT_HPP

#include <functional>

namespace higher_order {

// Returns a function that applies f to its argument times times.
std::function<int(int)> repeat(std::function<int(int)> f, int times);

// Returns n / 2 for an even n and 3n + 1 for an odd n.
int collatz_step(int n);

// Returns the value after times Collatz steps from start.
int collatz_after(int start, int times);

}  // namespace higher_order

#endif
