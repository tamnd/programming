// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-index-of-a-value/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { indexOf } from "./index_of.ts";

const cases: [string, number[], number, number][] = [
  ["middle", [4, 7, 1], 7, 1],
  ["absent", [4, 7, 1], 9, -1],
  ["empty", [], 0, -1],
  ["single", [5], 5, 0],
  ["single-absent", [5], 0, -1],
  ["first", [7, 8, 9], 7, 0],
  ["last", [7, 8, 9], 9, 2],
  ["duplicates", [3, 5, 3, 5], 5, 1],
  ["all-same", [2, 2, 2], 2, 0],
  ["negative", [0, -1, -2], -2, 2],
  ["limit", [2147483647, -2147483648], -2147483648, 1],
  ["limit-absent", [2147483647], -2147483648, -1],
];

for (const [name, items, value, want] of cases) {
  test(name, () => {
    const items0 = [...items];
    assert.equal(indexOf(items, value), want);
    assert.deepEqual(items, items0);
  });
}
