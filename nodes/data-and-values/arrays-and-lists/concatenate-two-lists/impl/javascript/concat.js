// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-two-lists/.

/**
 * @param {number[]} a
 * @param {number[]} b
 * @returns {number[]}
 */
export function concat(a, b) {
  // Spread syntax copies the items into a new array. a + b would make a string.
  return [...a, ...b];
}
