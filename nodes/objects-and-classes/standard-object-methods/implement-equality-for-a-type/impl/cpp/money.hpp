// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-equality-for-a-type/.

#ifndef MONEY_HPP
#define MONEY_HPP

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace money {

// An amount in cents and an upper-case currency code.
class Money {
  public:
    Money(std::int64_t cents, std::string_view currency);

    std::int64_t cents() const { return cents_; }
    const std::string& currency() const { return currency_; }

    // Compares the members in order; != comes from == (C++20).
    bool operator==(const Money&) const = default;

  private:
    std::int64_t cents_;
    std::string currency_;
};

int count_distinct(const std::vector<std::int64_t>& amounts, const std::vector<std::string>& currencies);

}  // namespace money

// std::unordered_set needs a hash; it uses the same members as operator==.
template <>
struct std::hash<money::Money> {
    std::size_t operator()(const money::Money& m) const noexcept {
        std::size_t h = std::hash<std::string>{}(m.currency());
        return h ^ (std::hash<std::int64_t>{}(m.cents()) + 0x9e3779b9 + (h << 6) + (h >> 2));
    }
};

#endif
