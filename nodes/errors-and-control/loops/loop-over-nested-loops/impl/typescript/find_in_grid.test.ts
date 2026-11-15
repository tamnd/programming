// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-nested-loops/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { findInGrid } from "./find_in_grid.ts";

const cases: [string, number[][], number, [number, number]][] = [
  ["middle", [[1, 2, 3], [4, 5, 6], [7, 8, 9]], 5, [1, 1]],
  ["first-twenty", [[3, 8, 12], [5, 20, 20], [20, 1, 4]], 20, [1, 1]],
  ["first-cell", [[9, 1], [2, 3]], 9, [0, 0]],
  ["last-cell", [[1, 2], [3, 9]], 9, [1, 1]],
  ["row-major", [[1, 2, 9], [9, 3, 4]], 9, [0, 2]],
  ["missing", [[1, 2], [3, 4]], 9, [-1, -1]],
  ["empty-grid", [], 1, [-1, -1]],
  ["empty-rows", [[], [], [5]], 5, [2, 0]],
  ["ragged", [[1], [2, 3, 4], [5, 6]], 6, [2, 1]],
  ["negative", [[-1, -2], [-3, -4]], -3, [1, 0]],
];

for (const [name, grid, target, want] of cases) {
  test(name, () => {
    assert.deepEqual(findInGrid(grid, target), want);
  });
}
