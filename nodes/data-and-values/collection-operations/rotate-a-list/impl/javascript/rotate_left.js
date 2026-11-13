// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rotate-a-list/.

/**
 * @param {number[]} items
 * @param {number} k
 * @returns {number[]}
 */
export function rotateLeft(items, k) {
  if (items.length === 0) {
    return [];
  }
  // k % items.length makes a k that is larger than the array work.
  const s = k % items.length;
  return [...items.slice(s), ...items.slice(0, s)];
}
