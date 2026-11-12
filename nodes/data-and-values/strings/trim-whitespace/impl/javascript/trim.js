// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/trim-whitespace/.

// trim returns s with the 6 ASCII white space characters removed from the
// start and the end. String.prototype.trim also removes U+00A0, U+3000, U+FEFF
// and the line terminators of Unicode, so the code uses a regular expression.
export function trim(s) {
  return s.replace(/^[ \t\n\v\f\r]+|[ \t\n\v\f\r]+$/g, "");
}

// trimStart returns s with the white space at the start removed.
export function trimStart(s) {
  return s.replace(/^[ \t\n\v\f\r]+/, "");
}
