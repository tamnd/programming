// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-caesar-or-rot13-cipher/.

/**
 * Moves each letter A to Z and a to z shift places along the alphabet, from
 * z back to a. Other characters do not change. A negative shift decodes, and
 * caesar(text, 13) is ROT13.
 */
export function caesar(text: string, shift: number): string {
  const k = ((shift % 26) + 26) % 26;
  return text.replace(/[A-Za-z]/g, (c: string) => {
    const base = c <= "Z" ? 65 : 97;
    return String.fromCharCode(((c.charCodeAt(0) - base + k) % 26) + base);
  });
}
