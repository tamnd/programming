// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/destructure-a-value/.

/**
 * Returns the Manhattan distance between the points p = [x, y] and q = [x, y].
 * @param {number[]} p
 * @param {number[]} q
 * @returns {number}
 */
export function manhattan(p, q) {
  const [x1, y1] = p;
  const [x2, y2] = q;
  return Math.abs(x1 - x2) + Math.abs(y1 - y2);
}
