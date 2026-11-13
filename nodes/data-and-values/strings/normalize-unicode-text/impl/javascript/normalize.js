// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/normalize-unicode-text/.

// normalize returns s in the form NFC or NFD. The 2 forms of the same text are
// different strings, so === is true only after this.
export function normalize(s, form) {
  return s.normalize(form);
}
