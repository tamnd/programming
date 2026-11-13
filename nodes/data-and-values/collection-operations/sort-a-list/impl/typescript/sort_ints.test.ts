// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { sortInts } from "./sort_ints.ts";

const cases: [string, number[], number[]][] = [
  ["small", [3, 1, 2], [1, 2, 3]],
  ["five", [5, 4, 1, 3, 2], [1, 2, 3, 4, 5]],
  ["empty", [], []],
  ["single", [1], [1]],
  ["sorted", [1, 2, 3], [1, 2, 3]],
  ["reversed", [9, 8, 7], [7, 8, 9]],
  ["duplicates", [2, 1, 2, 1], [1, 1, 2, 2]],
  ["negative", [-1, -10, 5], [-10, -1, 5]],
  ["digits", [10, 9, 100, 1], [1, 9, 10, 100]],
  ["extremes", [2147483647, -2147483648, 0], [-2147483648, 0, 2147483647]],
];

for (const [name, items, want] of cases) {
  test(name, () => {
    assert.deepEqual(sortInts(items), want);
  });
}
