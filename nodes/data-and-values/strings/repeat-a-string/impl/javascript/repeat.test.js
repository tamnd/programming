// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/repeat-a-string/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { repeat } from "./repeat.js";

// The case many has the 200 chars of 100 copies of ab in one literal.
const cases = [
  ["word", "ab", 3, "ababab"],
  ["one", "abc", 1, "abc"],
  ["dash", "-", 5, "-----"],
  ["zero", "abc", 0, ""],
  ["empty", "", 5, ""],
  ["empty-zero", "", 0, ""],
  ["many", "ab", 100, "abababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababab"],
  ["astral", "\u{1f642}", 3, "\u{1f642}\u{1f642}\u{1f642}"],
  ["combining", "e\u0301", 2, "e\u0301e\u0301"],
];

for (const [name, s, n, want] of cases) {
  test(name, () => {
    assert.equal(repeat(s, n), want);
  });
}
