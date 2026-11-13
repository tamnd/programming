// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-two-lists/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { concat } from "./concat.ts";

const cases: [string, number[], number[], number[]][] = [
  ["two-pairs", [1, 2], [3, 4], [1, 2, 3, 4]],
  ["uneven", [5], [6, 7, 8], [5, 6, 7, 8]],
  ["both-empty", [], [], []],
  ["left-empty", [], [1, 2], [1, 2]],
  ["right-empty", [1, 2], [], [1, 2]],
  ["duplicates", [1, 1], [1], [1, 1, 1]],
  ["order", [3, 1], [2], [3, 1, 2]],
  ["limit", [2147483647], [-2147483648], [2147483647, -2147483648]],
];

for (const [name, a, b, want] of cases) {
  test(name, () => {
    const a0 = [...a];
    const b0 = [...b];
    assert.deepEqual(concat(a, b), want);
    assert.deepEqual([a, b], [a0, b0]);
  });
}
