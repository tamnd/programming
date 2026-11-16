// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-to-and-from-roman-numerals/.

#ifndef ROMAN_HPP
#define ROMAN_HPP

#include <array>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

inline constexpr std::array<std::pair<int, std::string_view>, 13> roman_table{{
    {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
    {100, "C"},  {90, "XC"},  {50, "L"},  {40, "XL"},
    {10, "X"},   {9, "IX"},   {5, "V"},   {4, "IV"}, {1, "I"},
}};

// Returns the Roman numeral of n. Throws std::out_of_range if n is not in
// 1 to 3999.
inline std::string to_roman(int n) {
    if (n < 1 || n > 3999) {
        throw std::out_of_range("n is out of range");
    }
    std::string s;
    for (const auto& [value, symbol] : roman_table) {
        while (n >= value) {
            s += symbol;
            n -= value;
        }
    }
    return s;
}

constexpr int roman_value(char c) {
    switch (c) {
    case 'I': return 1;
    case 'V': return 5;
    case 'X': return 10;
    case 'L': return 50;
    case 'C': return 100;
    case 'D': return 500;
    case 'M': return 1000;
    default: return 0;
    }
}

// Returns the value of a Roman numeral in the standard form, or no value.
inline std::optional<int> from_roman(std::string_view s) {
    if (s.empty()) {
        return std::nullopt;
    }
    int total = 0;
    for (std::size_t i = 0; i < s.size(); ++i) {
        const int v = roman_value(s[i]);
        if (v == 0) {
            return std::nullopt;
        }
        const int next = i + 1 < s.size() ? roman_value(s[i + 1]) : 0;
        total += v < next ? -v : v;
    }
    if (total < 1 || total > 3999 || to_roman(total) != s) {
        return std::nullopt;
    }
    return total;
}

#endif
