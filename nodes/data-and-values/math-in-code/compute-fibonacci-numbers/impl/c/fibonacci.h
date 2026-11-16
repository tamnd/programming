// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-fibonacci-numbers/.

#ifndef FIBONACCI_H
#define FIBONACCI_H

// Returns F(n) for 0 <= n <= 92. Above 92, the sum overflows, which is
// undefined behavior for a signed type.
long long fibonacci(int n);

#endif
