// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-equality-for-a-type/.

#include "money.hpp"

#include <cctype>
#include <unordered_set>

namespace money {

Money::Money(std::int64_t cents, std::string_view currency) : cents_(cents), currency_(currency) {
    for (char& c : currency_) {
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));  // "eur" and "EUR" are the same currency
    }
}

int count_distinct(const std::vector<std::int64_t>& amounts, const std::vector<std::string>& currencies) {
    std::unordered_set<Money> seen;
    for (std::size_t i = 0; i < amounts.size(); i++) {
        seen.emplace(amounts[i], currencies[i]);
    }
    return static_cast<int>(seen.size());
}

}  // namespace money
