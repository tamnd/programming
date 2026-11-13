// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/map-over-a-list/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { squareAll } from "./square_all.js";

const cases = [
  ["small", [1, 2, 3], [1, 4, 9]],
  ["tens", [5, 10], [25, 100]],
  ["empty", [], []],
  ["single", [7], [49]],
  ["negative", [-3, -1], [9, 1]],
  ["zero", [0], [0]],
  ["limit", [46340, -46340], [2147395600, 2147395600]],
  ["order", [3, 1, 2], [9, 1, 4]],
  ["duplicates", [2, 2], [4, 4]],
];

for (const [name, items, want] of cases) {
  test(name, () => {
    assert.deepEqual(squareAll(items), want);
  });
}
