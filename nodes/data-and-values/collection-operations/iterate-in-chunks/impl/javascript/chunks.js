// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-in-chunks/.

/**
 * @param {number[]} items
 * @param {number} size at least 1
 * @returns {number[][]}
 */
export function chunks(items, size) {
  const out = [];
  for (let i = 0; i < items.length; i += size) {
    // slice makes a copy and stops at the end of the array.
    out.push(items.slice(i, i + size));
  }
  return out;
}
