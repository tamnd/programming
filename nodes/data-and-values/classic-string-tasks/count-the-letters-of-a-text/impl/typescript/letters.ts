// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-letters-of-a-text/.

/**
 * Returns the counts of the letters a to z in text, in the order of the
 * alphabet. Upper case letters count as lower case letters.
 */
export function countLetters(text: string): number[] {
  const counts = new Array(26).fill(0);
  for (const c of text.toLowerCase()) {
    const i = c.charCodeAt(0) - 97;
    if (i >= 0 && i < 26) {
      counts[i]++;
    }
  }
  return counts;
}
