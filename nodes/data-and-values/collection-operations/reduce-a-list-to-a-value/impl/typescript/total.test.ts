// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reduce-a-list-to-a-value/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { total } from "./total.ts";

const cases: [string, number[], number][] = [
  ["small", [1, 2, 3, 4], 10],
  ["mixed", [10, -3, 5], 12],
  ["empty", [], 0],
  ["single", [7], 7],
  ["negative", [-1, -2, -3], -6],
  ["zero-sum", [5, -5], 0],
  ["max", [2147483647], 2147483647],
  ["min", [-2147483648], -2147483648],
  ["to-the-limit", [2147483646, 1], 2147483647],
  ["partial-overflow", [2147483647, 1, -1], 2147483647],
  ["partial-underflow", [-2147483648, -1, 1], -2147483648],
];

for (const [name, items, want] of cases) {
  test(name, () => {
    assert.deepEqual(total(items), want);
  });
}
