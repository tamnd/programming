// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-a-substring/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { replace, replaceFirst } from "./replace.ts";

// U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
const all = [
  ["all", "a-b-c", "-", "+", "a+b+c"],
  ["word", "Hello, world", "world", "there", "Hello, there"],
  ["none", "abc", "x", "y", "abc"],
  ["empty-s", "", "a", "b", ""],
  ["delete", "a-b-c", "-", "", "abc"],
  ["new-contains-old", "aXa", "X", "XXX", "aXXXa"],
  ["overlap", "aaaa", "aa", "b", "bb"],
  ["overlap-odd", "aaa", "aa", "b", "ba"],
  ["case-sensitive", "Hello hello", "hello", "bye", "Hello bye"],
  ["non-ascii", "naïve caf\u00e9", "\u00e9", "e", "naïve cafe"],
  ["astral", "a\u{1f642}b\u{1f642}", "\u{1f642}", "!", "a!b!"],
  ["combining", "cafe\u0301", "e", "E", "cafE\u0301"],
  ["precomposed", "caf\u00e9", "e", "E", "caf\u00e9"],
  ["dot", "a.b.c", ".", "-", "a-b-c"],
  ["dollar", "price", "price", "$&!", "$&!"],
  ["ampersand", "a-b", "-", "&", "a&b"],
  ["backslash", "a/b", "/", "\\", "a\\b"],
];
const first = [
  ["first-only", "a-b-c", "-", "+", "a+b-c"],
  ["first-none", "abc", "x", "y", "abc"],
  ["first-overlap", "aaa", "aa", "b", "ba"],
  ["first-astral", "\u{1f642}\u{1f642}", "\u{1f642}", "x", "x\u{1f642}"],
  ["first-dollar", "a.b", ".", "$&", "a$&b"],
];

for (const [name, s, old, nw, want] of all) {
  test(name, () => {
    assert.equal(replace(s, old, nw), want);
  });
}
for (const [name, s, old, nw, want] of first) {
  test(name, () => {
    assert.equal(replaceFirst(s, old, nw), want);
  });
}
