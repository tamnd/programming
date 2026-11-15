// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-a-collection/.

/**
 * Returns the sum of the items, using a for...of loop.
 * @param {number[]} items
 * @returns {number}
 */
export function total(items) {
  let s = 0;
  // for...of gives the items. for...in would give the keys as strings.
  for (const x of items) {
    s += x;
  }
  return s;
}
