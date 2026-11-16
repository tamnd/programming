// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-fixed-decimals/.

#ifndef FIXED_HPP
#define FIXED_HPP

#include <format>
#include <string>

// Returns x with exactly digits digits after the decimal point. std::format
// rounds the exact binary value of x and does not use the locale.
inline std::string fixed(double x, int digits) {
    return std::format("{:.{}f}", x, digits);
}

#endif
