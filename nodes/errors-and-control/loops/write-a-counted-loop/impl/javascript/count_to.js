// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-counted-loop/.

/**
 * Returns the array [1, 2, ..., n].
 * @param {number} n
 * @returns {number[]}
 */
export function countTo(n) {
  const out = [];
  for (let i = 1; i <= n; i++) {
    out.push(i);
  }
  return out;
}
