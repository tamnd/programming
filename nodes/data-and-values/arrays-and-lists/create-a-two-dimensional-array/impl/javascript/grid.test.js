// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-two-dimensional-array/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { grid } from "./grid.js";

const cases = [
  ["two-by-three", 2, 3, 0, [[0, 0, 0], [0, 0, 0]]],
  ["three-by-one", 3, 1, 7, [[7], [7], [7]]],
  ["no-rows", 0, 3, 1, []],
  ["no-cols", 2, 0, 1, [[], []]],
  ["one-cell", 1, 1, 5, [[5]]],
  ["one-row", 1, 4, 2, [[2, 2, 2, 2]]],
  ["one-col", 4, 1, 2, [[2], [2], [2], [2]]],
  ["negative", 2, 2, -3, [[-3, -3], [-3, -3]]],
  ["min-int", 2, 2, -2147483648, [[-2147483648, -2147483648], [-2147483648, -2147483648]]],
  ["max-int", 1, 2, 2147483647, [[2147483647, 2147483647]]],
];

for (const [name, rows, cols, value, want] of cases) {
  test(name, () => {
    const got = grid(rows, cols, value);
    assert.deepEqual(got, want);
    if (rows >= 2 && cols >= 1) {
      got[0][0] = value ^ 1;
      assert.equal(got[1][0], value);
    }
  });
}
