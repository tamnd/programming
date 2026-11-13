// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-length-of-an-array/.

#include "length.hpp"

#include <cstddef>
#include <span>
#include <string>

std::size_t length(std::span<const std::string> items) {
    // size is unsigned. Use std::ssize when the code subtracts from it.
    return items.size();
}
