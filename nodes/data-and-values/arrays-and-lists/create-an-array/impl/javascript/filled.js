// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-an-array/.

/**
 * @param {number} n
 * @param {number} value
 * @returns {number[]}
 */
export function filled(n, value) {
  // new Array(n) has n empty slots. fill puts value in each one.
  return new Array(n).fill(value);
}
