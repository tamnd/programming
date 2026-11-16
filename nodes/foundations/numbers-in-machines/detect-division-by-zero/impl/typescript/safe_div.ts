// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-division-by-zero/.

// 64-bit values are bigint values. bigint division rounds toward zero, and a
// zero divisor throws a RangeError, so test the divisor first.
export function safeDiv(a: bigint, b: bigint): bigint | null {
  if (b === 0n) return null;
  const q = a / b;
  return BigInt.asIntN(64, q) === q ? q : null;
}
