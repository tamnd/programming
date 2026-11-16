// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-equality-for-a-type/.

#include "money.hpp"

#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<std::int64_t>& amounts,
                  const std::vector<std::string>& currencies, int want) {
    if (money::count_distinct(amounts, currencies) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

static void expect(std::string_view name, bool ok) {
    if (!ok) {
        std::cerr << "check " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("two-equal", {1250, 1250}, {"EUR", "EUR"}, 1);
    check("different-amounts", {100, 200, 100}, {"USD", "USD", "USD"}, 2);
    check("mixed", {500, 500, 500, 700}, {"EUR", "USD", "EUR", "EUR"}, 3);
    check("empty", {}, {}, 0);
    check("case", {999, 999, 999}, {"gbp", "GBP", "Gbp"}, 1);
    check("other-currency", {0, 0, 0}, {"EUR", "USD", "JPY"}, 3);
    check("negative", {-5, 5, -5}, {"EUR", "EUR", "EUR"}, 2);
    check("limit", {1000000000000, -1000000000000, 1000000000000}, {"usd", "USD", "USD"}, 2);

    money::Money a{1250, "EUR"}, b{1250, "eur"};
    expect("reflexive", a == a);
    expect("symmetric", a == b && b == a);
    expect("not equal", !(a != b));
    expect("hash", std::hash<money::Money>{}(a) == std::hash<money::Money>{}(b));
    expect("other amount", a != money::Money(1251, "EUR"));
    expect("other currency", a != money::Money(1250, "USD"));
    std::unordered_map<money::Money, std::string> names{{money::Money(500, "USD"), "lunch"}};
    expect("map key", names.at(money::Money(500, "usd")) == "lunch");

    if (failures == 0) {
        std::cout << "all cases passed\n";
    }
    return failures == 0 ? 0 : 1;
}
