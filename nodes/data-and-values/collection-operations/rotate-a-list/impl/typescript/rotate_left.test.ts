// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rotate-a-list/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { rotateLeft } from "./rotate_left.ts";

const cases: [string, number[], number, number[]][] = [
  ["by-two", [1, 2, 3, 4, 5], 2, [3, 4, 5, 1, 2]],
  ["by-one", [1, 2, 3], 1, [2, 3, 1]],
  ["zero", [1, 2, 3], 0, [1, 2, 3]],
  ["full-turn", [1, 2, 3], 3, [1, 2, 3]],
  ["more-than-length", [1, 2, 3], 4, [2, 3, 1]],
  ["much-larger", [1, 2, 3], 2000000000, [3, 1, 2]],
  ["empty", [], 5, []],
  ["empty-zero", [], 0, []],
  ["single", [7], 3, [7]],
  ["duplicates", [1, 1, 2], 1, [1, 2, 1]],
  ["negative-items", [-1, -2, -3], 1, [-2, -3, -1]],
];

for (const [name, items, k, want] of cases) {
  test(name, () => {
    assert.deepEqual(rotateLeft(items, k), want);
  });
}
