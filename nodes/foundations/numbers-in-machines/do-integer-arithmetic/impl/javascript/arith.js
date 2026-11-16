// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-integer-arithmetic/.

// The values are numbers. A number is a 64-bit float, but the ranges of the
// contract keep each result below 2 ** 53, so each result is exact.
export function arith(a, b, n) {
  // a / b is a float division; Math.trunc rounds the quotient toward zero.
  // % is a remainder with the sign of the dividend.
  return [a + b, a - b, a * b, Math.trunc(a / b), a % b, a ** n];
}
