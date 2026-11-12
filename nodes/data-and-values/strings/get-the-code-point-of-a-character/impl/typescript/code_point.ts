// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-code-point-of-a-character/.

// codePoint returns the code point of the one character in c. charCodeAt(0)
// would give only the first UTF-16 code unit of a code point above U+FFFF.
export function codePoint(c: string): number {
  return c.codePointAt(0)!;
}
