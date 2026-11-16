// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/take-the-first-n-items/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { take, takeNaturals } from "./take.ts";

const cases: [string, number[], number, number[]][] = [
  ["first-three", [1, 2, 3, 4, 5], 3, [1, 2, 3]],
  ["first-two", [10, 20, 30, 40], 2, [10, 20]],
  ["zero", [1, 2], 0, []],
  ["empty", [], 3, []],
  ["more-than-length", [1, 2], 5, [1, 2]],
  ["equal-length", [4, 5, 6], 3, [4, 5, 6]],
  ["one", [9, 8], 1, [9]],
  ["negative", [-1, -2, -3], 2, [-1, -2]],
  ["extremes", [-2147483648, 2147483647, 0], 2, [-2147483648, 2147483647]],
];

const infinite: [string, number, number, number[]][] = [
  ["from-zero", 0, 5, [0, 1, 2, 3, 4]],
  ["from-ten", 10, 3, [10, 11, 12]],
  ["none", 7, 0, []],
  ["one", 0, 1, [0]],
  ["large-start", 999998, 2, [999998, 999999]],
];

for (const [name, items, n, want] of cases) {
  test(name, () => {
    assert.deepEqual(take(items, n), want);
  });
}

for (const [name, start, n, want] of infinite) {
  test(`naturals ${name}`, () => {
    assert.deepEqual(takeNaturals(start, n), want);
  });
}

test("take returns a copy", () => {
  const items = [1, 2, 3];
  take(items, 2).push(99);
  assert.deepEqual(items, [1, 2, 3]);
});
