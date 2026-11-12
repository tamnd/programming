// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-substring/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { substring } from "./substring.js";

// U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
const cases = [
  ["middle", "Hello, world", 7, 12, "world"],
  ["prefix", "Hello, world", 0, 5, "Hello"],
  ["one-char", "abc", 1, 2, "b"],
  ["empty", "", 0, 0, ""],
  ["whole", "abc", 0, 3, "abc"],
  ["empty-range", "abc", 1, 1, ""],
  ["at-end", "abc", 3, 3, ""],
  ["non-ascii", "naïve café", 2, 5, "ïve"],
  ["cjk", "日本語テキスト", 3, 7, "テキスト"],
  ["astral", "a\u{1F642}b\u{1F643}c", 1, 4, "\u{1F642}b\u{1F643}"],
  ["astral-only", "\u{1F642}\u{1F643}", 1, 2, "\u{1F643}"],
  ["combining", "e\u0301t\u00e9", 0, 1, "e"],
  ["combining-mark", "e\u0301t\u00e9", 1, 3, "\u0301t"],
];

for (const [name, s, start, end, want] of cases) {
  test(name, () => {
    assert.equal(substring(s, start, end), want);
  });
}
