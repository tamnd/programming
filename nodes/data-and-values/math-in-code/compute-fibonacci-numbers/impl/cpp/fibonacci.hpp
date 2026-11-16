// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-fibonacci-numbers/.

#ifndef FIBONACCI_HPP
#define FIBONACCI_HPP

// Returns F(n) for 0 <= n <= 92. constexpr lets the compiler compute the
// Fibonacci number of a constant.
constexpr long long fibonacci(int n) {
    if (n == 0) {
        return 0;
    }
    long long a = 0;
    long long b = 1;
    for (int i = 1; i < n; i++) {
        const long long next = a + b;
        a = b;
        b = next;
    }
    return b;
}

#endif
