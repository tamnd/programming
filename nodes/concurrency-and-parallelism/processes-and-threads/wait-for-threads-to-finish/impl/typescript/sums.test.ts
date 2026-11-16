// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wait-for-threads-to-finish/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { parallelSums } from "./sums.ts";

const many = Array.from({ length: 32 }, (_, i) => [i, i, i]);

const cases: [string, number[][], number[]][] = [
  ["two", [[1, 2, 3], [4, 5]], [6, 9]],
  ["three", [[10], [20, 30], [-5, 5]], [10, 50, 0]],
  ["empty", [], []],
  ["empty-chunk", [[], [7]], [0, 7]],
  ["single", [[42]], [42]],
  ["negative", [[-1, -2], [-3]], [-3, -3]],
  ["limit", [[1e9, 1e9, 1e9], [-1e9, -1e9, -1e9]], [3e9, -3e9]],
  ["many", many, many.map((c) => c[0] * 3)],
];

for (const [name, chunks, want] of cases) {
  test(name, async () => {
    assert.deepEqual(await parallelSums(chunks), want);
  });
}
