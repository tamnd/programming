// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-substring/.

/**
 * Returns the code points of s from index start up to, not including, end.
 * @param {string} s
 * @param {number} start
 * @param {number} end
 * @returns {string}
 */
export function substring(s, start, end) {
  // s.slice counts UTF-16 code units. The string iterator yields code
  // points, so the code slices the array of code points.
  return Array.from(s).slice(start, end).join("");
}
