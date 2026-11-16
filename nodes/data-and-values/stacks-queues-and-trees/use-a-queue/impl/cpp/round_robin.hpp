// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-queue/.

#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

// finish_order returns the job numbers in the order in which round-robin
// finishes them.
std::vector<std::size_t> finish_order(const std::vector<std::int32_t> &work, std::int32_t slice);
