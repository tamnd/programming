// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-permutations-of-a-list/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { permutations } from "./permutations.js";

const cases = [
  ["three", [1, 2, 3], [[1, 2, 3], [1, 3, 2], [2, 1, 3], [2, 3, 1], [3, 1, 2], [3, 2, 1]]],
  ["two", [5, 6], [[5, 6], [6, 5]]],
  ["empty", [], [[]]],
  ["single", [7], [[7]]],
  ["duplicates", [1, 1, 2], [[1, 1, 2], [1, 2, 1], [1, 1, 2], [1, 2, 1], [2, 1, 1], [2, 1, 1]]],
  ["unsorted", [3, 1, 2], [[3, 1, 2], [3, 2, 1], [1, 3, 2], [1, 2, 3], [2, 3, 1], [2, 1, 3]]],
  ["extremes", [-2147483648, 2147483647], [[-2147483648, 2147483647], [2147483647, -2147483648]]],
];

for (const [name, items, want] of cases) {
  test(name, () => {
    assert.deepEqual(permutations(items), want);
  });
}
