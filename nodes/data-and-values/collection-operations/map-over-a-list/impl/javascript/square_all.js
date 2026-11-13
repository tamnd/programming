// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/map-over-a-list/.

/**
 * @param {number[]} items
 * @returns {number[]}
 */
export function squareAll(items) {
  // map makes a new array and does not change items.
  return items.map((x) => x * x);
}
