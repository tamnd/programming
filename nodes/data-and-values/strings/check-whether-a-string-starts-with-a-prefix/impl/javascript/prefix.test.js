// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-starts-with-a-prefix/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { endsWith, startsWith } from "./prefix.js";

// U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
const starts = [
  ["yes", "Hello, world", "Hello", true],
  ["no", "Hello, world", "world", false],
  ["whole", "abc", "abc", true],
  ["empty-prefix", "abc", "", true],
  ["empty-s", "", "a", false],
  ["both-empty", "", "", true],
  ["longer-prefix", "ab", "abc", false],
  ["case-sensitive", "Hello", "hello", false],
  ["non-ascii", "caf\u00e9 au lait", "caf\u00e9", true],
  ["cjk", "日本語", "日本", true],
  ["astral", "\u{1f642}\u{1f643}", "\u{1f642}", true],
  ["astral-no", "\u{1f642}", "\u{1f643}", false],
  ["combining", "cafe\u0301", "cafe", true],
  ["precomposed", "caf\u00e9", "cafe", false],
];
const ends = [
  ["ends-yes", "report.txt", ".txt", true],
  ["ends-no", "report.txt", ".csv", false],
  ["ends-empty", "abc", "", true],
  ["ends-longer", "txt", "a.txt", false],
  ["ends-case", "A.TXT", ".txt", false],
  ["ends-astral", "a\u{1f642}", "\u{1f642}", true],
  ["ends-combining", "cafe\u0301", "e", false],
];

for (const [name, s, prefix, want] of starts) {
  test(name, () => {
    assert.equal(startsWith(s, prefix), want);
  });
}
for (const [name, s, suffix, want] of ends) {
  test(name, () => {
    assert.equal(endsWith(s, suffix), want);
  });
}
