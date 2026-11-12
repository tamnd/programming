// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/join-a-list-of-strings/.

#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <string_view>

// join returns the items with sep between each two.
std::string join(std::span<const std::string_view> items, std::string_view sep);

// join_numbers returns the decimal form of each item, with sep between each two.
std::string join_numbers(std::span<const std::int32_t> items, std::string_view sep);
