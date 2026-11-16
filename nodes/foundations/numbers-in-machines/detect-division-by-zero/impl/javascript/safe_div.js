// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-division-by-zero/.

// 64-bit values are BigInt values. BigInt division rounds toward zero, and a
// zero divisor throws a RangeError, so test the divisor first. The quotient
// of the smallest 64-bit value by -1 is 2 ** 63, which does not fit in 64
// bits; BigInt.asIntN(64, q) changes q only in that case.
export function safeDiv(a, b) {
  if (b === 0n) return null;
  const q = a / b;
  return BigInt.asIntN(64, q) === q ? q : null;
}
