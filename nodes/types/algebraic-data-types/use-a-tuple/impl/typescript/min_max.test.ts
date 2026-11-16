// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-tuple/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { minMax } from "./min_max.ts";

const cases: [string, number[], [number, number]][] = [
  ["mixed", [3, 1, 2], [1, 3]],
  ["longer", [4, 9, -2, 7, 0], [-2, 9]],
  ["one", [5], [5, 5]],
  ["negative", [-3, -1, -7], [-7, -1]],
  ["all-same", [2, 2, 2], [2, 2]],
  ["int32-extremes", [2147483647, 0, -2147483648], [-2147483648, 2147483647]],
  ["sorted", [1, 2, 3, 4], [1, 4]],
  ["reversed", [4, 3, 2, 1], [1, 4]],
];

for (const [name, numbers, want] of cases) {
  test(name, () => {
    const before = [...numbers];
    const [lo, hi] = minMax(numbers);
    assert.deepEqual([lo, hi], want);
    assert.deepEqual(numbers, before);
  });
}
