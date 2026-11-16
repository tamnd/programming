// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-factorial/.

#ifndef FACTORIAL_H
#define FACTORIAL_H

// Returns n! for 0 <= n <= 20. Above 20, the product overflows, which is
// undefined behavior for a signed type.
long long factorial(int n);

#endif
