// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-tuple/.

// A readonly tuple type: an array of exactly two numbers that the caller
// cannot change.
export type MinMax = readonly [min: number, max: number];

export function minMax(numbers: readonly number[]): MinMax {
  let lo = numbers[0];
  let hi = numbers[0];
  for (const n of numbers) {
    if (n < lo) lo = n;
    if (n > hi) hi = n;
  }
  return [lo, hi];
}
