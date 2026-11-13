// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-length-of-an-array/.

#pragma once

#include <cstddef>
#include <span>
#include <string>

// length returns the number of items in items.
std::size_t length(std::span<const std::string> items);
