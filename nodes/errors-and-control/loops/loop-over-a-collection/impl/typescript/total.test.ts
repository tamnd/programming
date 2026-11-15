// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-a-collection/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { total } from "./total.ts";

const cases: [string, number[], number][] = [
  ["three", [1, 2, 3], 6],
  ["mixed", [10, -4, 7, 0, 3], 16],
  ["empty", [], 0],
  ["single", [7], 7],
  ["negative", [-5, 2, -1], -4],
  ["limit", [2147483647, 2147483647, 2], 4294967296],
  ["limit-low", [-2147483648, -2147483648], -4294967296],
];

for (const [name, items, want] of cases) {
  test(name, () => {
    assert.equal(total(items), want);
  });
}
