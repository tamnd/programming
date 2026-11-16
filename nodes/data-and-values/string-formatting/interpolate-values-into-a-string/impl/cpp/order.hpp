// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/interpolate-values-into-a-string/.

#ifndef ORDER_HPP
#define ORDER_HPP

#include <cstdint>
#include <format>
#include <string>
#include <string_view>

// Returns "Order {name}: count item(s), total count*price". The doubled
// braces are literal braces.
inline std::string describe_order(std::string_view name, std::int64_t count, double price) {
    const char* ending = count == 1 ? "" : "s";
    return std::format("Order {{{}}}: {} item{}, total {:.2f}", name, count, ending,
                       static_cast<double>(count) * price);
}

#endif
