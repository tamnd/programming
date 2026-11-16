// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-thread-local-storage/.

// Count on a thread-local counter in one thread for each count, with no lock and no shared counter.

#include "tls.hpp"

#include <cstddef>
#include <thread>

namespace tls {

namespace {
// Each thread has its own copy of this variable. The copy is made when the thread starts
// and destroyed when the thread ends.
thread_local std::int32_t counter = 0;
}  // namespace

std::int32_t bump() { return ++counter; }

std::int32_t current() { return counter; }

std::vector<std::int32_t> count_per_thread(const std::vector<std::int32_t>& counts) {
    std::vector<std::int32_t> results(counts.size(), 0);
    {
        std::vector<std::jthread> threads;
        threads.reserve(counts.size());
        for (std::size_t i = 0; i < counts.size(); i++) {
            threads.emplace_back([&counts, &results, i] {
                for (std::int32_t k = 0; k < counts[i]; k++) {
                    bump();
                }
                // Each thread writes only its own element. The join makes the write visible to the caller.
                results[i] = current();
            });
        }
    }  // The jthread destructors join every thread here.
    return results;
}

}  // namespace tls
