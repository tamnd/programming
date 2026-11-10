// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-string/.

// Returns the code points of s in the opposite order. The spread syntax uses
// the string iterator, which yields code points, so a surrogate pair stays
// together. s.split("") would yield UTF-16 code units and break the pairs.
export function reverseCodePoints(s: string): string {
  return [...s].reverse().join("");
}

const graphemes = new Intl.Segmenter(undefined, { granularity: "grapheme" });

// Returns the extended grapheme clusters of s in the opposite order. A letter
// keeps its combining marks, and an emoji sequence stays in one piece.
export function reverseGraphemes(s: string): string {
  return Array.from(graphemes.segment(s), (part) => part.segment)
    .reverse()
    .join("");
}
