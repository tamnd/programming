// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-with-a-custom-key/.

export function sortByLength(words: readonly string[]): string[] {
  // toSorted returns a new array. The comparison gives a negative number, 0 or
  // a positive number: the length first, then the code units of the words.
  return words.toSorted((a, b) => a.length - b.length || (a < b ? -1 : a > b ? 1 : 0));
}
