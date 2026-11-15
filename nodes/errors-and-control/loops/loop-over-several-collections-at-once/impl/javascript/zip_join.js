// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-several-collections-at-once/.

/**
 * Joins the items at each index, up to the end of the shortest list.
 * @param {string[]} lower
 * @param {string[]} upper
 * @param {number[]} digits
 * @returns {string[]}
 */
export function zipJoin(lower, upper, digits) {
  const out = [];
  // An index past the end of a shorter list would give undefined.
  const n = Math.min(lower.length, upper.length, digits.length);
  for (let i = 0; i < n; i++) {
    out.push(`${lower[i]}${upper[i]}${digits[i]}`);
  }
  return out;
}
