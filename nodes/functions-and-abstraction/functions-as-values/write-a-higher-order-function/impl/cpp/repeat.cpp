// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-higher-order-function/.

#include "repeat.hpp"

#include <utility>

namespace higher_order {

std::function<int(int)> repeat(std::function<int(int)> f, int times) {
    return [f = std::move(f), times](int x) {
        for (int i = 0; i < times; ++i) {
            x = f(x);
        }
        return x;
    };
}

int collatz_step(int n) {
    return n % 2 == 0 ? n / 2 : 3 * n + 1;
}

int collatz_after(int start, int times) {
    return repeat(collatz_step, times)(start);
}

}  // namespace higher_order
