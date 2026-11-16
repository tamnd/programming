// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/swap-two-values/.

/**
 * Returns a copy of xs with the items at i and j swapped. The right side
 * makes a new array of the two values first, and the destructuring
 * assignment then puts them into the two places.
 */
export function swapItems(xs, i, j) {
  const out = [...xs];
  [out[i], out[j]] = [out[j], out[i]];
  return out;
}
