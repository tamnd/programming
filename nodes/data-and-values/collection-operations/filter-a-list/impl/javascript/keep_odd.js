// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-a-list/.

/**
 * @param {number[]} items
 * @returns {number[]}
 */
export function keepOdd(items) {
  // Use !== 0, not === 1: the remainder of a negative item can be negative.
  return items.filter((x) => x % 2 !== 0);
}
