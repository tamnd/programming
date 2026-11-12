// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-length-of-a-string/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { lengths } from "./stringLength.ts";

const cases: [string, string, number[]][] = [
  ["ascii", "hello", [5, 5, 5]],
  ["words", "Hello, world", [12, 12, 12]],
  ["two-byte", "naïve", [6, 5, 5]],
  ["three-byte", "日本語", [9, 3, 3]],
  ["cyrillic", "Привет", [12, 6, 6]],
  ["empty", "", [0, 0, 0]],
  ["single", "a", [1, 1, 1]],
  ["euro-sign", "€", [3, 1, 1]],
  ["astral", "🙂", [4, 2, 1]],
  ["mixed", "a🙂b", [6, 4, 3]],
  ["combining-mark", "e\u0301", [3, 2, 2]],
  ["precomposed", "é", [2, 1, 1]],
  ["flag", "🇫🇷", [8, 4, 2]],
  ["zwj-family", "👨\u200d👩\u200d👧", [18, 8, 5]],
  ["nul", "a\u0000b", [3, 3, 3]],
];

for (const [name, s, want] of cases) {
  test(name, () => {
    assert.deepEqual(lengths(s), want);
  });
}
