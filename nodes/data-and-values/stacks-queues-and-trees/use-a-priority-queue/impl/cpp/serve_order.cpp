// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-priority-queue/.

#include "serve_order.hpp"

#include <queue>

std::vector<std::size_t> serve_order(const std::vector<std::int32_t> &priorities) {
    // std::priority_queue puts the largest element for its comparator on
    // top. The comparator is a "less than": a is less than b if a comes out
    // after b.
    auto after = [&priorities](std::size_t a, std::size_t b) {
        if (priorities[a] != priorities[b]) {
            return priorities[a] < priorities[b];
        }
        return a > b;
    };
    std::priority_queue<std::size_t, std::vector<std::size_t>, decltype(after)> queue(after);
    for (std::size_t i = 0; i < priorities.size(); i++) {
        queue.push(i);
    }
    std::vector<std::size_t> served;
    while (!queue.empty()) {
        served.push_back(queue.top()); // read top() first:
        queue.pop();                   // pop() returns nothing
    }
    return served;
}
