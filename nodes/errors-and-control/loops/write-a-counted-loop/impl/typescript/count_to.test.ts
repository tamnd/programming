// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-counted-loop/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { countTo } from "./count_to.ts";

const cases: [string, number, number[]][] = [
  ["ten", 10, [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]],
  ["three", 3, [1, 2, 3]],
  ["zero", 0, []],
  ["one", 1, [1]],
  ["two", 2, [1, 2]],
];

for (const [name, n, want] of cases) {
  test(name, () => {
    assert.deepEqual(countTo(n), want);
  });
}
