// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-factorial/.

/** Returns n! as a BigInt for a non-negative BigInt n. */
export function factorial(n) {
  if (n < 0n) {
    throw new RangeError("n is negative");
  }
  let f = 1n;
  for (let i = 2n; i <= n; i++) {
    f *= i;
  }
  return f;
}
