// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-an-enumeration/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { Day, cycle } from "./cycle.js";

const cases = [
  ["weekdays", "monday", 3, ["monday", "tuesday", "wednesday"]],
  ["wrap", "friday", 4, ["friday", "saturday", "sunday", "monday"]],
  ["zero-steps", "monday", 0, []],
  ["more-than-a-week", "sunday", 9, ["sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday", "monday"]],
  ["unknown", "funday", 3, []],
  ["wrong-case", "Friday", 2, []],
  ["number", "3", 2, []],
  ["empty-name", "", 1, []],
];

for (const [name, start, steps, want] of cases) {
  test(name, () => assert.deepEqual(cycle(start, steps), want));
}

test("the enum object is frozen", () => {
  assert.equal(Object.isFrozen(Day), true);
  assert.equal(Object.keys(Day).length, 7);
});
