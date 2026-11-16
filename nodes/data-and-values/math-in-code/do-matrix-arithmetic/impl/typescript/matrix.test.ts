// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-matrix-arithmetic/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { type Matrix, matrixMultiply, transpose } from "./matrix.ts";

const cases: [string, Matrix, Matrix, number[][]][] = [
  ["two-by-two", [[1, 2], [3, 4]], [[5, 6], [7, 8]], [[19, 22], [43, 50]]],
  ["two-by-three-times-three-by-two", [[1, 2, 3], [4, 5, 6]], [[7, 8], [9, 10], [11, 12]], [[58, 64], [139, 154]]],
  ["one-by-one", [[3]], [[4]], [[12]]],
  ["row-times-column", [[1, 2, 3]], [[4], [5], [6]], [[32]]],
  ["column-times-row", [[1], [2], [3]], [[4, 5, 6]], [[4, 5, 6], [8, 10, 12], [12, 15, 18]]],
  ["identity", [[1, 0], [0, 1]], [[5, 6], [7, 8]], [[5, 6], [7, 8]]],
  ["other-order", [[5, 6], [7, 8]], [[1, 2], [3, 4]], [[23, 34], [31, 46]]],
  ["zero-matrix", [[0, 0], [0, 0]], [[1, 2], [3, 4]], [[0, 0], [0, 0]]],
  ["negative-numbers", [[-1, 2], [3, -4]], [[2, 0], [1, -1]], [[0, -2], [2, 4]]],
  ["beyond-32-bit", [[100000, 0], [0, 100000]], [[100000], [3]], [[10000000000], [300000]]],
];

for (const [name, a, b, want] of cases) {
  test(name, () => {
    const before = structuredClone([a, b]);
    assert.deepEqual(matrixMultiply(a, b), want);
    assert.deepEqual([a, b], before);
  });
}

test("transpose", () => {
  assert.deepEqual(transpose([[1, 2, 3], [4, 5, 6]]), [[1, 4], [2, 5], [3, 6]]);
});

test("sizes-must-match", () => {
  assert.throws(() => matrixMultiply([[1, 2]], [[1, 2]]), RangeError);
});
