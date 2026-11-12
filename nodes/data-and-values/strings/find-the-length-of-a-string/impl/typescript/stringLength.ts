// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-length-of-a-string/.

const encoder = new TextEncoder();

/**
 * Returns the length of s in UTF-8 bytes, in UTF-16 code units and in code
 * points.
 */
export function lengths(s: string): [number, number, number] {
  // s.length counts UTF-16 code units. The string iterator yields code points.
  return [encoder.encode(s).length, s.length, [...s].length];
}
