// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-empty/.

// isEmpty reports whether s has no code points.
export function isEmpty(s) {
  return s.length === 0;
}

// isBlank reports whether each code point of s is ASCII white space. trim()
// also removes Unicode spaces, so the code uses a character class instead.
export function isBlank(s) {
  return /^[ \t\n\v\f\r]*$/.test(s);
}
