// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-number-is-prime/.

#ifndef IS_PRIME_HPP
#define IS_PRIME_HPP

// Returns true if n is prime, with trial division. i <= n / i does not
// overflow, but i * i can.
constexpr bool is_prime(long long n) {
    if (n < 2) {
        return false;
    }
    if (n % 2 == 0) {
        return n == 2;
    }
    for (long long i = 3; i <= n / i; i += 2) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

#endif
