// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wait-for-threads-to-finish/.

#include "sums.hpp"

#include <cstddef>
#include <numeric>
#include <thread>

namespace sums {

std::vector<std::int64_t> parallel_sums(const std::vector<std::vector<std::int64_t>>& chunks) {
    std::vector<std::int64_t> results(chunks.size());
    {
        std::vector<std::jthread> threads;
        threads.reserve(chunks.size());
        for (std::size_t i = 0; i < chunks.size(); i++) {
            threads.emplace_back([&results, &chunks, i] {
                results[i] = std::accumulate(chunks[i].begin(), chunks[i].end(), std::int64_t{0});
            });
        }
        // The destructor of each std::jthread joins its thread, here and also when emplace_back throws.
    }
    return results;
}

}  // namespace sums
