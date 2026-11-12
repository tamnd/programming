// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-for-a-substring/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { find, findLast } from "./search.ts";

// U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
const first = [
  ["found", "Hello, world", "world", 7],
  ["first-of-two", "abcabc", "bc", 1],
  ["not-found", "abc", "d", -1],
  ["empty-sub", "abc", "", 0],
  ["empty-s", "", "a", -1],
  ["both-empty", "", "", 0],
  ["longer-sub", "ab", "abc", -1],
  ["overlap", "aaaa", "aa", 0],
  ["case-sensitive", "Hello", "hello", -1],
  ["non-ascii", "naïve caf\u00e9", "caf\u00e9", 6],
  ["cjk", "日本語テキスト", "テキ", 3],
  ["astral", "a\u{1f642}b\u{1f642}c", "b", 2],
  ["astral-sub", "x\u{1f642}y", "\u{1f642}y", 1],
  ["combining", "cafe\u0301", "e", 3],
  ["precomposed", "cafe\u0301", "\u00e9", -1],
];
const last = [
  ["last-of-two", "abcabc", "bc", 4],
  ["last-not-found", "abc", "d", -1],
  ["last-empty-sub", "abc", "", 3],
  ["last-empty-s", "", "", 0],
  ["last-overlap", "aaaa", "aa", 2],
  ["last-astral", "\u{1f642}a\u{1f642}a", "a", 3],
  ["last-non-ascii", "caf\u00e9 caf\u00e9", "\u00e9", 8],
];

for (const [name, s, sub, want] of first) {
  test(name, () => {
    assert.equal(find(s, sub), want);
  });
}
for (const [name, s, sub, want] of last) {
  test(name, () => {
    assert.equal(findLast(s, sub), want);
  });
}
