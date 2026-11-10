// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-word-frequencies/.

// A word is a run of ASCII letters, ASCII digits and characters that are not ASCII.
const WORD = /[A-Za-z0-9\u{80}-\u{10FFFF}]+/gu;

/**
 * Returns the n most frequent words of text as [word, count] pairs, by count
 * from high to low, then by word in code point order.
 */
export function topWords(text: string, n: number): [string, number][] {
  const counts = new Map<string, number>();
  for (const [word] of text.matchAll(WORD)) {
    const key = word.toLowerCase();
    counts.set(key, (counts.get(key) ?? 0) + 1);
  }
  // < compares UTF-16 code units. Below U+D800 that is code point order.
  return [...counts]
    .sort(([a, x], [b, y]) => y - x || (a < b ? -1 : a > b ? 1 : 0))
    .slice(0, n);
}
