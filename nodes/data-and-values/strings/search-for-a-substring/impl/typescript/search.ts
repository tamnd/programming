// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-for-a-substring/.

// indexOf and lastIndexOf count UTF-16 code units. A match of a well-formed
// string starts at a code point, so the code counts the code points before it.
const codePoints = (s: string, end: number): number => Array.from(s.slice(0, end)).length;

/** Returns the code point index of the first match of sub in s, or -1. */
export function find(s: string, sub: string): number {
  const i = s.indexOf(sub);
  return i < 0 ? -1 : codePoints(s, i);
}

/** Returns the code point index of the last match of sub in s, or -1. */
export function findLast(s: string, sub: string): number {
  const i = s.lastIndexOf(sub);
  return i < 0 ? -1 : codePoints(s, i);
}
