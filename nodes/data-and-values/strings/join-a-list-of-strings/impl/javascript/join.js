// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/join-a-list-of-strings/.

/**
 * Returns the items with sep between each two.
 * @param {string[]} items
 * @param {string} sep
 * @returns {string}
 */
export function join(items, sep) {
  // join with no argument uses a comma, so the code always passes sep.
  return items.join(sep);
}

/**
 * Returns the decimal form of each item, with sep between each two.
 * @param {number[]} items
 * @param {string} sep
 * @returns {string}
 */
export function joinNumbers(items, sep) {
  return items.map(String).join(sep);
}
