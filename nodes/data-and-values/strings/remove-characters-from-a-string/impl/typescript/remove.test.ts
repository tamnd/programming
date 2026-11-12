// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-characters-from-a-string/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { removeChars } from "./remove.ts";

// A character is a code point. U+1F642 and U+1F643 have the same high surrogate in UTF-16, and U+00E0 and U+00E9 have the same first byte in UTF-8.
const cases: [string, string, string, string][] = [
  ["one-char", "banana", "a", "bnn"],
  ["set", "hello, world", "lo", "he, wrd"],
  ["punct", "a-b_c.d", "-_.", "abcd"],
  ["empty", "", "abc", ""],
  ["empty-set", "hello", "", "hello"],
  ["none-found", "hello", "xyz", "hello"],
  ["all", "aaa", "a", ""],
  ["adjacent", "aab", "a", "b"],
  ["astral", "a\u{1f642}b\u{1f642}", "\u{1f642}", "ab"],
  ["astral-other", "\u{1f642}\u{1f643}", "\u{1f643}", "\u{1f642}"],
  ["non-ascii", "\u00e0\u00e9", "\u00e9", "\u00e0"],
  ["combining-mark", "e\u0301x", "\u0301", "ex"],
  ["combining-base", "e\u0301", "e", "\u0301"],
  ["composed", "e\u0301", "\u00e9", "e\u0301"],
];

for (const [name, s, chars, want] of cases) {
  test(name, () => {
    assert.equal(removeChars(s, chars), want);
  });
}
