// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-a-palindrome/.

/**
 * Returns true if the ASCII letters and digits of text, in lower case, read
 * the same backwards. Other characters do not count.
 */
export function isPalindrome(text: string): boolean {
  const s = text.toLowerCase().replace(/[^a-z0-9]/g, "");
  for (let i = 0, j = s.length - 1; i < j; i++, j--) {
    if (s[i] !== s[j]) {
      return false;
    }
  }
  return true;
}
