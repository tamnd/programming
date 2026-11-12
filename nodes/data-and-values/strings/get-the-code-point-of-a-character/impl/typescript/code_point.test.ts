// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-code-point-of-a-character/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { codePoint } from "./code_point.ts";

// U+1F642 is one code point, but it is 2 UTF-16 code units and 4 UTF-8 bytes. U+10FFFF is the largest code point.
const cases: [string, string, number][] = [
  ["upper", "A", 65],
  ["lower", "a", 97],
  ["digit", "0", 48],
  ["space", " ", 32],
  ["tab", "\t", 9],
  ["tilde", "~", 126],
  ["two-byte", "\u00e9", 233],
  ["three-byte", "\u20ac", 8364],
  ["cjk", "\u65e5", 26085],
  ["combining", "\u0301", 769],
  ["astral", "\u{1f642}", 128578],
  ["max", "\u{10ffff}", 1114111],
];

for (const [name, c, want] of cases) {
  test(name, () => {
    assert.equal(codePoint(c), want);
  });
}
