// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-tasks-on-a-thread-pool/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { stepsOnPool } from "./pool.ts";

function steps(n: number): number {
  let count = 0;
  while (n !== 1) {
    n = n % 2 === 0 ? n / 2 : 3 * n + 1;
    count++;
  }
  return count;
}

const many = Array.from({ length: 200 }, (_, i) => i + 1);

const cases: [string, number, number[], number[]][] = [
  ["four-jobs", 2, [1, 2, 3, 6], [0, 1, 7, 8]],
  ["long-runs", 3, [27, 97, 871], [111, 118, 178]],
  ["empty", 2, [], []],
  ["one-worker", 1, [7, 8, 9], [16, 3, 19]],
  ["more-workers-than-jobs", 8, [5, 6], [5, 8]],
  ["duplicates", 3, [27, 27, 27, 1], [111, 111, 111, 0]],
  ["wide-intermediate", 2, [113383, 837799], [247, 524]],
  ["many", 4, many, many.map(steps)],
];

for (const [name, workers, numbers, want] of cases) {
  test(name, async () => {
    assert.deepEqual(await stepsOnPool(workers, numbers), want);
  });
}
