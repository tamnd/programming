// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/change-the-case-of-a-string/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { toLower, toUpper } from "./case.ts";

// Only the ASCII letters change. U+0131 is the dotless i, U+0130 is the capital I with a dot, U+212A is the KELVIN SIGN, and U+FF41 is the fullwidth a.
const upper = [
  ["word", "hello", "HELLO"],
  ["mixed", "Hello, World!", "HELLO, WORLD!"],
  ["already", "ABC", "ABC"],
  ["empty", "", ""],
  ["digits-punct", "a1-b2_c3", "A1-B2_C3"],
  ["non-ascii", "caf\u00e9", "CAF\u00e9"],
  ["sharp-s", "stra\u00dfe", "STRA\u00dfE"],
  ["dotless-i", "\u0131 i", "\u0131 I"],
  ["greek", "\u03c3", "\u03c3"],
  ["fullwidth", "\uff41", "\uff41"],
  ["astral", "a\u{1f642}b", "A\u{1f642}B"],
  ["combining", "cafe\u0301", "CAFE\u0301"],
];
const lower = [
  ["lower-word", "HELLO", "hello"],
  ["lower-mixed", "Hello, World!", "hello, world!"],
  ["lower-empty", "", ""],
  ["lower-dotted-i", "\u0130", "\u0130"],
  ["lower-sigma", "\u03a3\u0391\u03a3", "\u03a3\u0391\u03a3"],
  ["lower-kelvin", "\u212a", "\u212a"],
  ["lower-astral", "A\u{1f642}B", "a\u{1f642}b"],
];

for (const [name, s, want] of upper) {
  test(name, () => {
    assert.equal(toUpper(s), want);
  });
}
for (const [name, s, want] of lower) {
  test(name, () => {
    assert.equal(toLower(s), want);
  });
}
