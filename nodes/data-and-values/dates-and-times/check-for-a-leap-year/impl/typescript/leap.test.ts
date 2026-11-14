// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-for-a-leap-year/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { isLeapYear } from "./leap.ts";

const cases: [string, number, boolean][] = [
  ["divisible-by-4", 2024, true],
  ["not-divisible-by-4", 2023, false],
  ["century", 1900, false],
  ["divisible-by-400", 2000, true],
  ["next-century", 2100, false],
  ["year-1600", 1600, true],
  ["year-1700", 1700, false],
  ["year-2400", 2400, true],
  ["year-1", 1, false],
  ["year-4", 4, true],
  ["year-100", 100, false],
  ["year-1500-julian-leap", 1500, false],
  ["year-1582", 1582, false],
  ["year-9999", 9999, false],
  ["year-zero", 0, true],
  ["year-minus-1", -1, false],
  ["year-minus-4", -4, true],
  ["year-minus-100", -100, false],
  ["year-minus-400", -400, true],
];

for (const [name, year, want] of cases) {
  test(name, () => {
    assert.equal(isLeapYear(year), want);
  });
}

test("leap days in 400 years", () => {
  let count = 0;
  for (let year = 2000; year < 2400; year++) {
    if (isLeapYear(year)) count++;
  }
  assert.equal(count, 97);
});
