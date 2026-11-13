// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-two-dimensional-array/.

/**
 * Returns rows arrays of cols items, each item value. No two rows share an array.
 * @param {number} rows
 * @param {number} cols
 * @param {number} value
 * @returns {number[][]}
 */
export function grid(rows, cols, value) {
  // The function makes a new row for each row. new Array(rows).fill(row)
  // would put one row object in every slot.
  return Array.from({ length: rows }, () => new Array(cols).fill(value));
}
