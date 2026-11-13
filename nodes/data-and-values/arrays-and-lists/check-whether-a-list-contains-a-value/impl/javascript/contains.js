// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

/**
 * Returns true if some item of items is equal to value.
 * @param {number[]} items
 * @param {number} value
 * @returns {boolean}
 */
export function contains(items, value) {
  // includes returns a boolean. indexOf returns a position, and 0 is falsy.
  return items.includes(value);
}
