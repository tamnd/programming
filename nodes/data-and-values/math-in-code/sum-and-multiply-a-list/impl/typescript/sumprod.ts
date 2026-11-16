// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sum-and-multiply-a-list/.

/** Returns [sum, product] of a list of bigint values. */
export function sumAndProduct(items: bigint[]): [bigint, bigint] {
  const sum = items.reduce((a, x) => a + x, 0n);
  const product = items.reduce((a, x) => a * x, 1n);
  return [sum, product];
}
