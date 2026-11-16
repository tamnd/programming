// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-fibonacci-numbers/.

/** Returns F(n) as a BigInt for a non-negative BigInt n. */
export function fibonacci(n) {
  if (n < 0n) {
    throw new RangeError("n is negative");
  }
  if (n === 0n) {
    return 0n;
  }
  let a = 0n;
  let b = 1n;
  for (let i = 1n; i < n; i++) {
    [a, b] = [b, a + b];
  }
  return b;
}
