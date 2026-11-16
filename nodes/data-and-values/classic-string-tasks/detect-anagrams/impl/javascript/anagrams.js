// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-anagrams/.

function count(counts, text, step) {
  for (const c of text.toLowerCase()) {
    const i = c.charCodeAt(0) - 97;
    if (i >= 0 && i < 26) {
      counts[i] += step;
    }
  }
}

/**
 * Returns true if a and b hold each letter from a to z the same number of
 * times. Case, spaces, punctuation and digits do not count.
 */
export function areAnagrams(a, b) {
  const counts = new Array(26).fill(0);
  count(counts, a, 1);
  count(counts, b, -1);
  return counts.every((n) => n === 0);
}
