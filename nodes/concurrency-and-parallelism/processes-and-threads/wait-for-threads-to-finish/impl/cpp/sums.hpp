// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wait-for-threads-to-finish/.

#pragma once

#include <cstdint>
#include <vector>

namespace sums {

// Sums each chunk on its own std::jthread, waits for all of them and returns the sums in order.
// Throws std::system_error if a thread cannot be started.
std::vector<std::int64_t> parallel_sums(const std::vector<std::vector<std::int64_t>>& chunks);

}  // namespace sums
