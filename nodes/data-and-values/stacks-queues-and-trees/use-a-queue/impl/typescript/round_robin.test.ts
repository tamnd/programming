// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-queue/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { finishOrder } from "./round_robin.ts";

const cases: [string, number[], number, number[]][] = [
  ["example", [3, 1, 2], 1, [1, 2, 0]],
  ["slice-two", [4, 1, 3, 2], 2, [1, 3, 0, 2]],
  ["empty", [], 3, []],
  ["one", [7], 2, [0]],
  ["big-slice", [3, 1, 2], 10, [0, 1, 2]],
  ["slice-one", [2, 2, 1], 1, [2, 0, 1]],
  ["equal-work", [2, 2, 2], 1, [0, 1, 2]],
  ["long-job-first", [10, 1, 1], 3, [1, 2, 0]],
];

for (const [name, work, slice, want] of cases) {
  test(name, () => {
    const before = [...work];
    assert.deepEqual(finishOrder(work, slice), want);
    assert.deepEqual(work, before);
  });
}
