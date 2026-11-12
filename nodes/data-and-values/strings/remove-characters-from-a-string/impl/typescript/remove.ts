// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-characters-from-a-string/.

// removeChars returns s without the code points of chars. The iterator of a
// string gives code points, not UTF-16 code units, so a code point above
// U+FFFF is one element of the Set and of the array.
export function removeChars(s: string, chars: string): string {
  const remove = new Set(chars);
  return Array.from(s)
    .filter((c) => !remove.has(c))
    .join("");
}
