// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-factorial/.

#ifndef FACTORIAL_HPP
#define FACTORIAL_HPP

// Returns n! for 0 <= n <= 20. constexpr lets the compiler compute the
// factorial of a constant.
constexpr long long factorial(int n) {
    long long f = 1;
    for (int i = 2; i <= n; i++) {
        f *= i;
    }
    return f;
}

#endif
