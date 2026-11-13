// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-list/.

/**
 * Returns a new array with value at position index. index items.length appends.
 * @param {number[]} items
 * @param {number} index
 * @param {number} value
 * @returns {number[]}
 */
export function insertAt(items, index, value) {
  if (!Number.isInteger(index) || index < 0 || index > items.length) {
    throw new RangeError(`index ${index} is out of range`);
  }
  // slice copies, so items does not change. splice would change it in place.
  return [...items.slice(0, index), value, ...items.slice(index)];
}
