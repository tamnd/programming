// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-constant/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { SECONDS_PER_DAY, secondsIn } from "./seconds.js";

const cases = [
  ["one-day", 1, 86400],
  ["one-week", 7, 604800],
  ["one-year", 365, 31536000],
  ["zero", 0, 0],
  ["two-days", 2, 172800],
  ["leap-year", 366, 31622400],
  ["last-32-bit-day", 24855, 2147472000],
  ["past-32-bits", 24856, 2147558400],
  ["million-days", 1000000, 86400000000],
];

for (const [name, days, want] of cases) {
  test(name, () => {
    assert.equal(secondsIn(days), want);
  });
}

test("seconds per day", () => {
  assert.equal(SECONDS_PER_DAY, 86400);
});
