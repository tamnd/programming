// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/zip-two-lists/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { zipPairs } from "./zip_pairs.js";

const cases = [
  ["same-length", [1, 2, 3], [4, 5, 6], [[1, 4], [2, 5], [3, 6]]],
  ["tens", [10, 20], [30, 40], [[10, 30], [20, 40]]],
  ["empty", [], [], []],
  ["first-empty", [], [1, 2], []],
  ["second-empty", [1, 2], [], []],
  ["first-shorter", [1], [7, 8], [[1, 7]]],
  ["second-shorter", [1, 2, 3], [9], [[1, 9]]],
  ["negative", [-1, -2], [3, -4], [[-1, 3], [-2, -4]]],
  ["extremes", [-2147483648, 2147483647], [2147483647, -2147483648], [[-2147483648, 2147483647], [2147483647, -2147483648]]],
  ["duplicates", [5, 5], [5, 5], [[5, 5], [5, 5]]],
];

for (const [name, a, b, want] of cases) {
  test(name, () => {
    assert.deepEqual(zipPairs(a, b), want);
  });
}
