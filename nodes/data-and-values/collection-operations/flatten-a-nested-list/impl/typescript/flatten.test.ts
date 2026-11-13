// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/flatten-a-nested-list/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { flatten } from "./flatten.ts";

const cases: [string, number[][], number[]][] = [
  ["three", [[1, 2], [3], [4, 5, 6]], [1, 2, 3, 4, 5, 6]],
  ["two", [[7], [8, 9]], [7, 8, 9]],
  ["empty", [], []],
  ["empty-inner", [[], [1], []], [1]],
  ["all-empty", [[], []], []],
  ["single", [[5]], [5]],
  ["one-list", [[1, 2, 3]], [1, 2, 3]],
  ["negative", [[-1, -2], [3]], [-1, -2, 3]],
  ["extremes", [[-2147483648], [2147483647, 0]], [-2147483648, 2147483647, 0]],
];

for (const [name, lists, want] of cases) {
  test(name, () => {
    assert.deepEqual(flatten(lists), want);
  });
}
