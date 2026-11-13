// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-graphemes-of-a-string/.

const segmenter = new Intl.Segmenter(undefined, { granularity: "grapheme" });

// countGraphemes returns the number of extended grapheme clusters in s.
// s.length counts UTF-16 code units, and [...s].length counts code points.
export function countGraphemes(s) {
  return [...segmenter.segment(s)].length;
}
