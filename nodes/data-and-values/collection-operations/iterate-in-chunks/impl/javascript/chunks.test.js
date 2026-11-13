// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-in-chunks/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { chunks } from "./chunks.js";

const cases = [
  ["pairs", [1, 2, 3, 4, 5], 2, [[1, 2], [3, 4], [5]]],
  ["triples", [1, 2, 3, 4, 5, 6], 3, [[1, 2, 3], [4, 5, 6]]],
  ["empty", [], 3, []],
  ["size-one", [7, 8, 9], 1, [[7], [8], [9]]],
  ["size-larger", [1, 2], 5, [[1, 2]]],
  ["size-equal", [1, 2, 3], 3, [[1, 2, 3]]],
  ["last-shorter", [1, 2, 3, 4], 3, [[1, 2, 3], [4]]],
  ["negative", [-1, -2, -3], 2, [[-1, -2], [-3]]],
  ["extremes", [-2147483648, 2147483647, 0], 2, [[-2147483648, 2147483647], [0]]],
];

for (const [name, items, size, want] of cases) {
  test(name, () => {
    assert.deepEqual(chunks(items, size), want);
  });
}
