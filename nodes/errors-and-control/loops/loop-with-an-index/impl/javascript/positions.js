// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-an-index/.

/**
 * Returns the index of each item that equals target.
 * @param {string[]} items
 * @param {string} target
 * @returns {number[]}
 */
export function positions(items, target) {
  const out = [];
  // entries() gives [index, item] pairs.
  for (const [i, x] of items.entries()) {
    if (x === target) {
      out.push(i);
    }
  }
  return out;
}
