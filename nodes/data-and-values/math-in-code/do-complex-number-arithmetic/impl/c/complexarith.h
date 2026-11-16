// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-complex-number-arithmetic/.

#ifndef COMPLEX_ARITHMETIC_H
#define COMPLEX_ARITHMETIC_H

// Sets out to the sum, the product and the quotient of a and b, then abs(a)
// and phase(a), as 8 numbers. a and b are pairs {re, im}.
void complex_arithmetic(const double a[2], const double b[2], double out[8]);

#endif
