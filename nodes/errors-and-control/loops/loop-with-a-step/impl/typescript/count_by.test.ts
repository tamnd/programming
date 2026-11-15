// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-a-step/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { countBy } from "./count_by.ts";

const cases: [string, number, number, number, number[]][] = [
  ["by-two", 2, 8, 2, [2, 4, 6, 8]],
  ["down", 10, 0, -1, [10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0]],
  ["uneven", 1, 10, 3, [1, 4, 7, 10]],
  ["past-stop", 0, 9, 4, [0, 4, 8]],
  ["down-by-three", 10, 1, -3, [10, 7, 4, 1]],
  ["empty-up", 5, 1, 1, []],
  ["empty-down", 1, 5, -1, []],
  ["single", 3, 3, 7, [3]],
  ["negative", -3, 3, 2, [-3, -1, 1, 3]],
  ["limit-up", 2147483640, 2147483647, 5, [2147483640, 2147483645]],
  ["limit-down", -2147483640, -2147483648, -5, [-2147483640, -2147483645]],
  ["big-step", 0, 10, 2147483647, [0]],
];

for (const [name, start, stop, step, want] of cases) {
  test(name, () => {
    assert.deepEqual(countBy(start, stop, step), want);
  });
}
