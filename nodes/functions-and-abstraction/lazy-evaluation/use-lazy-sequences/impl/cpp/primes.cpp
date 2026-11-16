// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-lazy-sequences/.

#include "primes.hpp"

#include <ranges>

bool is_prime(int k) {
    if (k < 2) {
        return false;
    }
    for (int d = 2; d * d <= k; d++) {
        if (k % d == 0) {
            return false;
        }
    }
    return true;
}

std::vector<int> first_primes(std::size_t n) {
    // iota(2) has no end; filter and take are views, so they compute a value
    // only when the loop asks for it.
    auto primes = std::views::iota(2) | std::views::filter(is_prime) | std::views::take(n);
    std::vector<int> out;
    for (int p : primes) {
        out.push_back(p);
    }
    return out;
}
