// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/measure-elapsed-time/.

#include "elapsed.hpp"

#include <format>

std::string elapsed_ms(std::int64_t start_ns, std::int64_t end_ns) {
    std::int64_t ns = end_ns - start_ns;
    // Integer division truncates, and ns is not negative.
    return std::format("{}.{:03} ms", ns / 1'000'000, ns / 1'000 % 1'000);
}
