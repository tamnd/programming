// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-items-of-each-kind/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { countItems } from "./count_items.ts";

const cases: [string, number[], [number, number][]][] = [
  ["mixed", [3, 1, 3, 2, 1, 3], [[1, 2], [2, 1], [3, 3]]],
  ["same", [5, 5, 5], [[5, 3]]],
  ["empty", [], []],
  ["single", [7], [[7, 1]]],
  ["negative", [-1, 2, -1, 0], [[-1, 2], [0, 1], [2, 1]]],
  ["distinct", [4, 2, 9], [[2, 1], [4, 1], [9, 1]]],
  ["order", [9, 8, 9], [[8, 1], [9, 2]]],
  ["zero", [0, 0], [[0, 2]]],
  ["extremes", [2147483647, -2147483648, 2147483647], [[-2147483648, 1], [2147483647, 2]]],
];

for (const [name, items, want] of cases) {
  test(name, () => {
    assert.deepEqual(countItems(items), want);
  });
}
