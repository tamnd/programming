// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-switch-or-match/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { daysInMonth } from "./days_in_month.ts";

const cases: [string, number, number][] = [
  ["january", 1, 31],
  ["february", 2, 28],
  ["april", 4, 30],
  ["july", 7, 31],
  ["november", 11, 30],
  ["december", 12, 31],
  ["zero", 0, 0],
  ["thirteen", 13, 0],
  ["minus-one", -1, 0],
];

for (const [name, month, want] of cases) {
  test(name, () => {
    assert.equal(daysInMonth(month), want);
  });
}
