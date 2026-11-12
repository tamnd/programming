// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/trim-whitespace/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { trimStart, trim } from "./trim.ts";

// The white space is the 6 ASCII characters: space, tab, line feed, vertical tab, form feed and carriage return. U+00A0, U+3000, U+0085 and U+FEFF stay.
const cases = [
  ["both-ends", "  hello  ", "hello"],
  ["tab-newline", "\thello world\n", "hello world"],
  ["none", "hello", "hello"],
  ["empty", "", ""],
  ["all-space", " \t\n\r ", ""],
  ["vt-ff", "\v\fx\f\v", "x"],
  ["other-control", "\u0001x\u0001", "\u0001x\u0001"],
  ["no-break-space", "\u00a0x\u00a0", "\u00a0x\u00a0"],
  ["ideographic-space", "\u3000x\u3000", "\u3000x\u3000"],
  ["next-line", "\u0085x\u0085", "\u0085x\u0085"],
  ["bom", "\ufeffx", "\ufeffx"],
  ["astral", " \u{1f642} ", "\u{1f642}"],
  ["combining", " \u0301x ", "\u0301x"],
];
const start = [
  ["start-both-ends", "  hello  ", "hello  "],
  ["start-tab-newline", "\n\tx", "x"],
  ["start-empty", "", ""],
  ["start-all-space", "   ", ""],
  ["start-no-break-space", "\u00a0 x", "\u00a0 x"],
  ["start-end-only", "x \t", "x \t"],
];

for (const [name, s, want] of cases) {
  test(name, () => {
    assert.equal(trim(s), want);
  });
}
for (const [name, s, want] of start) {
  test(name, () => {
    assert.equal(trimStart(s), want);
  });
}
