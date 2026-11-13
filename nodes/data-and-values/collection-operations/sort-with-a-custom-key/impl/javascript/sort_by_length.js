// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-with-a-custom-key/.

/**
 * @param {string[]} words
 * @returns {string[]}
 */
export function sortByLength(words) {
  // toSorted returns a new array. The comparison gives a negative number, 0 or
  // a positive number: the length first, then the code units of the words.
  return words.toSorted((a, b) => a.length - b.length || (a < b ? -1 : a > b ? 1 : 0));
}
