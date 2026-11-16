// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/destructure-a-value/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { manhattan, type Point } from "./manhattan.ts";

const cases: [string, Point, Point, number][] = [
  ["first", [1, 2], [4, 6], 7],
  ["on-a-line", [0, 0], [10, 0], 10],
  ["down-left", [5, 5], [2, 1], 7],
  ["mixed-signs", [3, -1], [-2, 4], 10],
  ["same-point", [7, 7], [7, 7], 0],
  ["negative", [-3, -4], [-1, -1], 5],
  ["large", [1000000, -1000000], [-1000000, 1000000], 4000000],
];

for (const [name, p, q, want] of cases) {
  test(name, () => {
    assert.equal(manhattan(p, q), want);
  });
}
