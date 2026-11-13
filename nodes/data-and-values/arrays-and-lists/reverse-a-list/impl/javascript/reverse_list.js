// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-list/.

/**
 * Returns a new array of the items in the opposite order.
 * @param {number[]} items
 * @returns {number[]}
 */
export function reverseList(items) {
  // items.reverse() would change items and return the same array.
  return items.toReversed();
}
