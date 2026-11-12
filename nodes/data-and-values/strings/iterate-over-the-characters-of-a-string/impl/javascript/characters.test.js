// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-the-characters-of-a-string/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { characters } from "./characters.js";

// A character is a code point. U+1F642 is one character, but two UTF-16 code units and four UTF-8 bytes. A combining mark, a ZWJ and each regional indicator is a character of its own.
const cases = [
  ["ascii", "abc", ["a", "b", "c"]],
  ["word", "hello", ["h", "e", "l", "l", "o"]],
  ["space", "a b", ["a", " ", "b"]],
  ["empty", "", []],
  ["single", "x", ["x"]],
  ["non-ascii", "h\u00e9", ["h", "\u00e9"]],
  ["cjk", "\u65e5\u672c", ["\u65e5", "\u672c"]],
  ["astral", "a\u{1f642}b", ["a", "\u{1f642}", "b"]],
  ["combining", "e\u0301", ["e", "\u0301"]],
  ["regional-indicator", "\u{1f1fa}\u{1f1f8}", ["\u{1f1fa}", "\u{1f1f8}"]],
  ["zwj", "\u{1f469}\u200d\u{1f4bb}", ["\u{1f469}", "\u200d", "\u{1f4bb}"]],
  ["control", "a\tb\nc", ["a", "\t", "b", "\n", "c"]],
];

for (const [name, s, want] of cases) {
  test(name, () => {
    assert.deepEqual(characters(s), want);
  });
}
