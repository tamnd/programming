// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-queue/.

#include "round_robin.hpp"

#include <algorithm>
#include <queue>

std::vector<std::size_t> finish_order(const std::vector<std::int32_t> &work, std::int32_t slice) {
    std::vector<std::int32_t> left = work;
    std::queue<std::size_t> queue;
    for (std::size_t i = 0; i < work.size(); i++) {
        queue.push(i); // enqueue at the back
    }
    std::vector<std::size_t> done;
    while (!queue.empty()) {
        std::size_t job = queue.front(); // read the front first:
        queue.pop();                     // pop() returns nothing
        left[job] -= std::min(slice, left[job]);
        if (left[job] == 0) {
            done.push_back(job);
        } else {
            queue.push(job);
        }
    }
    return done;
}
