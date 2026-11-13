// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/partition-a-list/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { partitionOdd } from "./partition_odd.ts";

const cases: [string, number[], [number[], number[]]][] = [
  ["mixed", [1, 2, 3, 4, 5], [[1, 3, 5], [2, 4]]],
  ["tens", [10, 15, 20, 25], [[15, 25], [10, 20]]],
  ["empty", [], [[], []]],
  ["all-odd", [1, 3], [[1, 3], []]],
  ["all-even", [2, 4], [[], [2, 4]]],
  ["negative", [-3, -2, -1, 0], [[-3, -1], [-2, 0]]],
  ["zero", [0], [[], [0]]],
  ["order", [9, 8, 1, 2], [[9, 1], [8, 2]]],
  ["extremes", [-2147483648, 2147483647], [[2147483647], [-2147483648]]],
  ["duplicates", [3, 3, 2, 2], [[3, 3], [2, 2]]],
];

for (const [name, items, want] of cases) {
  test(name, () => {
    assert.deepEqual(partitionOdd(items), want);
  });
}
