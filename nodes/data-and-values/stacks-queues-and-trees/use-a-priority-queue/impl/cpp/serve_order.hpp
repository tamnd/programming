// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-priority-queue/.

#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

// serve_order returns the request numbers in serve order: the largest
// priority first, and for equal priorities, the smaller request number first.
std::vector<std::size_t> serve_order(const std::vector<std::int32_t> &priorities);
