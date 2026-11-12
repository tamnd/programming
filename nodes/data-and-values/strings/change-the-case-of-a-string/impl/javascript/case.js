// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/change-the-case-of-a-string/.

// toUpper returns s with a to z changed to A to Z. toUpperCase changes each
// letter of Unicode, and "ß".toUpperCase() is "SS", so the function
// changes only the ASCII letters.
export function toUpper(s) {
  return s.replace(/[a-z]+/g, (m) => m.toUpperCase());
}

// toLower returns s with A to Z changed to a to z.
export function toLower(s) {
  return s.replace(/[A-Z]+/g, (m) => m.toLowerCase());
}
