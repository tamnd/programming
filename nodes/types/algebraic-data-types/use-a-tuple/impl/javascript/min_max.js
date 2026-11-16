// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-tuple/.

// minMax returns [smallest, largest] of an array with at least one number.
// JavaScript has no tuple type, so the pair is an array of length 2.
export function minMax(numbers) {
  let lo = numbers[0];
  let hi = numbers[0];
  for (const n of numbers) {
    if (n < lo) lo = n;
    if (n > hi) hi = n;
  }
  return [lo, hi];
}
