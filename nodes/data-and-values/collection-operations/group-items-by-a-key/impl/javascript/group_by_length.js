// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/group-items-by-a-key/.

/**
 * @param {string[]} words
 * @returns {[number, string[]][]}
 */
export function groupByLength(words) {
  // Map.groupBy keeps the keys in the order of their first word.
  const groups = Map.groupBy(words, (word) => word.length);
  // Compare the lengths as numbers. sort() without a function compares strings.
  return [...groups].sort((p, q) => p[0] - q[0]);
}
