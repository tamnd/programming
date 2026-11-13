// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list/.

/**
 * @param {number[]} items
 * @returns {number[]}
 */
export function sortInts(items) {
  // A copy, so items does not change. Without the compare function, sort
  // compares the numbers as strings, and 10 comes before 9.
  return [...items].sort((a, b) => a - b);
}
