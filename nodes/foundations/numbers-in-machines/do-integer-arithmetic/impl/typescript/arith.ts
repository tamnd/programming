// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-integer-arithmetic/.

// The values are numbers. The ranges of the contract keep each result below
// 2 ** 53, so each result is exact.
export function arith(a: number, b: number, n: number): number[] {
  // a / b is a float division; Math.trunc rounds the quotient toward zero.
  return [a + b, a - b, a * b, Math.trunc(a / b), a % b, a ** n];
}
