// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-function/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { clamp } from "./clamp.ts";

const cases: [string, number, number, number, number][] = [
  ["inside", 5, 1, 10, 5],
  ["below", -3, 0, 10, 0],
  ["above", 42, 0, 10, 10],
  ["at-low", 0, 0, 10, 0],
  ["at-high", 10, 0, 10, 10],
  ["equal-bounds", 7, 3, 3, 3],
  ["negative-range", -50, -20, -10, -20],
  ["zero", 0, -5, 5, 0],
  ["int-max", 2147483647, -100, 100, 100],
  ["int-min", -2147483648, -100, 100, -100],
  ["full-range", -2147483648, -2147483648, 2147483647, -2147483648],
];

for (const [name, value, low, high, want] of cases) {
  test(name, () => {
    assert.equal(clamp(value, low, high), want);
  });
}
