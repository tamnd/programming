// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-tasks-on-a-thread-pool/.

#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace pool {

// Returns the number of steps that take n to 1.
std::int32_t collatz_steps(std::int64_t n);

// Submits one job for each number to a pool of `workers` threads and returns the step counts in order.
// Throws std::system_error if a thread cannot be started.
std::vector<std::int32_t> steps_on_pool(std::size_t workers, const std::vector<std::int32_t>& numbers);

}  // namespace pool
