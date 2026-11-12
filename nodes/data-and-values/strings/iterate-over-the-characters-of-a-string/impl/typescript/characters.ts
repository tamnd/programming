// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-the-characters-of-a-string/.

// characters returns the code points of s. The string iterator gives code
// points, so U+1F642 is one element. s.split("") would give UTF-16 code units.
export function characters(s: string): string[] {
  return Array.from(s);
}
