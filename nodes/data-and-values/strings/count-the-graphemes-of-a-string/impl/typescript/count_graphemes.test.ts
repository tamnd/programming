// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-graphemes-of-a-string/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { countGraphemes } from "./count_graphemes.ts";

// A flag is 2 regional indicators, and U+1F469 U+200D U+1F4BB is 3 code points. Each of them is one grapheme.
const cases: [string, string, number][] = [
  ["empty", "", 0],
  ["ascii", "abc", 3],
  ["precomposed", "\u00e9", 1],
  ["emoji", "\u{1f642}", 1],
  ["combining", "e\u0301", 1],
  ["two-marks", "e\u0301\u0302", 1],
  ["leading-mark", "\u0301a", 2],
  ["crlf", "\r\n", 1],
  ["lfcr", "\n\r", 2],
  ["cr-mark", "\r\u0301", 2],
  ["soft-hyphen", "a\u00adb", 3],
  ["flag", "\u{1f1eb}\u{1f1f7}", 1],
  ["three-indicators", "\u{1f1eb}\u{1f1f7}\u{1f1ea}", 2],
  ["two-flags", "\u{1f1eb}\u{1f1f7}\u{1f1ea}\u{1f1f8}", 2],
  ["zwj-sequence", "\u{1f469}\u200d\u{1f4bb}", 1],
  ["zwj-after-letter", "a\u200d\u{1f642}", 2],
  ["skin-tone", "\u{1f44d}\u{1f3fd}", 1],
  ["variation-selector", "\u2764\ufe0f", 1],
];

for (const [name, s, want] of cases) {
  test(name, () => {
    assert.equal(countGraphemes(s), want);
  });
}
