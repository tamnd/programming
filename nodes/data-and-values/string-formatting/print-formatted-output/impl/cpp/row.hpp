// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-formatted-output/.

#ifndef ROW_HPP
#define ROW_HPP

#include <cstdint>
#include <format>
#include <string>
#include <string_view>

// Returns name in 10 columns on the left, qty in 5 on the right and price
// with 2 decimals in 8 on the right. A width is a minimum, so a long value is
// not cut.
inline std::string format_row(std::string_view name, std::int64_t qty, double price) {
    return std::format("{:<10}|{:5}|{:8.2f}", name, qty, price);
}

#endif
