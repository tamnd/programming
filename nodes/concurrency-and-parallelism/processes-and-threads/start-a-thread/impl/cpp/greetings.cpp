// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/start-a-thread/.

#include "greetings.hpp"

#include <cstddef>
#include <thread>

namespace greetings {

std::vector<std::string> thread_greetings(const std::vector<std::string_view>& names) {
    std::vector<std::string> results(names.size());
    std::vector<std::thread> threads;
    threads.reserve(names.size());
    try {
        for (std::size_t i = 0; i < names.size(); i++) {
            // The lambda gets copies of i and the name; it writes only its own slot.
            threads.emplace_back([&results, i, name = names[i]] { results[i] = "hello from " + std::string(name); });
        }
    } catch (...) {
        // A joinable std::thread must not be destroyed, so join the started threads first.
        for (auto& thread : threads) {
            thread.join();
        }
        throw;
    }
    // Start all threads first, then wait for each one.
    for (auto& thread : threads) {
        thread.join();
    }
    return results;
}

}  // namespace greetings
