// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-set/.

/**
 * @param {string[]} words
 * @returns {string[]}
 */
export function toggle(words) {
  const seen = new Set();
  for (const word of words) {
    // delete returns true if the set had the word.
    if (!seen.delete(word)) {
      seen.add(word);
    }
  }
  // The default sort compares UTF-16 code units, which is the byte order for ASCII.
  return [...seen].sort();
}
