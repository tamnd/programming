// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-combinations-of-a-list/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { combinations } from "./combinations.js";

const cases = [
  ["two-of-four", [1, 2, 3, 4], 2, [[1, 2], [1, 3], [1, 4], [2, 3], [2, 4], [3, 4]]],
  ["three-of-four", [1, 2, 3, 4], 3, [[1, 2, 3], [1, 2, 4], [1, 3, 4], [2, 3, 4]]],
  ["zero-k", [1, 2], 0, [[]]],
  ["empty", [], 0, [[]]],
  ["k-too-large", [1, 2], 3, []],
  ["all", [1, 2, 3], 3, [[1, 2, 3]]],
  ["duplicates", [1, 1, 2], 2, [[1, 1], [1, 2], [1, 2]]],
  ["unsorted", [3, 1, 2], 2, [[3, 1], [3, 2], [1, 2]]],
  ["extremes", [-2147483648, 2147483647], 1, [[-2147483648], [2147483647]]],
];

for (const [name, items, k, want] of cases) {
  test(name, () => {
    assert.deepEqual(combinations(items, k), want);
  });
}
