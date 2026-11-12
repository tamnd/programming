// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-empty/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { isBlank, isEmpty } from "./empty.js";

// U+00A0 is NO-BREAK SPACE, U+200B is ZERO WIDTH SPACE, and U+3000 is IDEOGRAPHIC SPACE. None of them is ASCII white space.
const empty = [
  ["letter", "a", false],
  ["word", "hello", false],
  ["empty", "", true],
  ["space", " ", false],
  ["newline", "\n", false],
  ["nbsp", "\u00a0", false],
  ["zero-width", "\u200b", false],
  ["combining", "\u0301", false],
  ["astral", "\u{1f642}", false],
  ["cjk", "日", false],
];
const blank = [
  ["blank-empty", "", true],
  ["blank-space", " ", true],
  ["blank-mixed", " \t\n\r", true],
  ["blank-vt-ff", "\v\f", true],
  ["blank-letter", "a", false],
  ["blank-padded", "  a  ", false],
  ["blank-nbsp", "\u00a0", false],
  ["blank-zero-width", "\u200b", false],
  ["blank-ideographic", "\u3000", false],
  ["blank-astral", " \u{1f642} ", false],
];

for (const [name, s, want] of empty) {
  test(name, () => {
    assert.equal(isEmpty(s), want);
  });
}
for (const [name, s, want] of blank) {
  test(name, () => {
    assert.equal(isBlank(s), want);
  });
}
