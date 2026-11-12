// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-strings/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { compare, equals } from "./compare.ts";

// U+FF61 is HALFWIDTH IDEOGRAPHIC FULL STOP. It is before U+1F642 in code point order, but its UTF-16 code unit is after the high surrogate of U+1F642.
const order = [
  ["less", "apple", "banana", -1],
  ["equal", "apple", "apple", 0],
  ["greater", "pear", "apple", 1],
  ["both-empty", "", "", 0],
  ["empty-first", "", "a", -1],
  ["prefix", "app", "apple", -1],
  ["prefix-longer", "apple", "app", 1],
  ["upper-before-lower", "Zebra", "apple", -1],
  ["lower-after-upper", "a", "A", 1],
  ["digits", "10", "9", -1],
  ["non-ascii", "\u00e9", "z", 1],
  ["astral-after-bmp", "\uff61", "\u{1f642}", -1],
  ["astral", "\u{1f642}", "\u{1f643}", -1],
  ["combining", "cafe\u0301", "caf\u00e9", -1],
];
const equal = [
  ["eq-same", "abc", "abc", true],
  ["eq-case", "abc", "ABC", false],
  ["eq-empty", "", "", true],
  ["eq-combining", "cafe\u0301", "caf\u00e9", false],
  ["eq-astral", "\u{1f642}", "\u{1f642}", true],
  ["eq-trailing-space", "abc", "abc ", false],
];

for (const [name, a, b, want] of order) {
  test(name, () => {
    assert.equal(compare(a, b), want);
  });
}
for (const [name, a, b, want] of equal) {
  test(name, () => {
    assert.equal(equals(a, b), want);
  });
}
