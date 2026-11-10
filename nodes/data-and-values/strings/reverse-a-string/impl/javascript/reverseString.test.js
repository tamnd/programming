// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-string/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { reverseCodePoints, reverseGraphemes } from "./reverseString.js";

const codePointCases = [
  ["ascii", "hello", "olleh"],
  ["two-words", "abc def", "fed cba"],
  ["palindrome", "racecar", "racecar"],
  ["empty", "", ""],
  ["single", "a", "a"],
  ["two-byte", "naïve", "evïan"],
  ["three-byte", "日本語", "語本日"],
  ["astral", "a🙂b", "b🙂a"],
  ["two-astral", "🙂😀", "😀🙂"],
  ["combining-mark", "e\u0301x", "x\u0301e"],
  ["flag", "🇫🇷", "🇷🇫"],
];

const graphemeCases = [
  ["ascii", "abc", "cba"],
  ["empty", "", ""],
  ["combining-mark", "e\u0301x", "xe\u0301"],
  ["flags", "🇫🇷🇩🇪", "🇩🇪🇫🇷"],
  ["zwj-family", "a👨\u200d👩\u200d👧b", "b👨\u200d👩\u200d👧a"],
  ["skin-tone", "👍🏽!", "!👍🏽"],
  ["crlf", "a\r\nb", "b\r\na"],
  ["hangul-jamo", "\u1100\u1161x", "x\u1100\u1161"],
];

for (const [name, s, want] of codePointCases) {
  test(`reverseCodePoints ${name}`, () => {
    assert.equal(reverseCodePoints(s), want);
  });
}

for (const [name, s, want] of graphemeCases) {
  test(`reverseGraphemes ${name}`, () => {
    assert.equal(reverseGraphemes(s), want);
  });
}
