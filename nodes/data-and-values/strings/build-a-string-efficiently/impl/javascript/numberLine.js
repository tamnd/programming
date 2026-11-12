// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/build-a-string-efficiently/.

/**
 * Returns the numbers 1 to n, separated by commas.
 * @param {number} n
 * @returns {string}
 */
export function numberLine(n) {
  // Collect the parts in an array and join them once at the end.
  const parts = [];
  for (let i = 1; i <= n; i++) {
    parts.push(String(i));
  }
  return parts.join(",");
}
