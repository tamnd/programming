// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-strings/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { concat, concatNumber } from "./concatenate.ts";

const cases: [string, string, string, string][] = [
  ["hello-world", "Hello, ", "world", "Hello, world"],
  ["two-words", "good", "bye", "goodbye"],
  ["keep-spaces", "a ", " b", "a  b"],
  ["empty-both", "", "", ""],
  ["empty-left", "", "abc", "abc"],
  ["empty-right", "abc", "", "abc"],
  ["non-ascii", "naï", "ve", "naïve"],
  ["astral", "a", "🙂", "a🙂"],
  ["combining-mark", "e", "\u0301", "e\u0301"],
  ["flag-halves", "\u{1f1eb}", "\u{1f1f7}", "🇫🇷"],
  ["digit-strings", "1", "2", "12"],
];

const numberCases: [string, string, number, string][] = [
  ["positive", "item", 3, "item3"],
  ["zero", "n=", 0, "n=0"],
  ["negative", "x", -5, "x-5"],
  ["empty-prefix", "", 42, "42"],
  ["int32-max", "max=", 2147483647, "max=2147483647"],
  ["int32-min", "min=", -2147483648, "min=-2147483648"],
];

for (const [name, a, b, want] of cases) {
  test(name, () => {
    assert.equal(concat(a, b), want);
  });
}

for (const [name, s, n, want] of numberCases) {
  test(`number ${name}`, () => {
    assert.equal(concatNumber(s, n), want);
  });
}
