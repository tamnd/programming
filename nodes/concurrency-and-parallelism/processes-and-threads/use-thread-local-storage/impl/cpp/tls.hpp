// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-thread-local-storage/.

#pragma once

#include <cstdint>
#include <vector>

namespace tls {

// Adds 1 to the counter of the current thread and returns the new value.
std::int32_t bump();

// Returns the counter of the current thread.
std::int32_t current();

// Starts one thread for each count and returns the final counter of each thread, in order.
// Throws std::system_error if a thread cannot be started.
std::vector<std::int32_t> count_per_thread(const std::vector<std::int32_t>& counts);

}  // namespace tls
