// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { contains } from "./contains.ts";

const cases: [string, number[], number, boolean][] = [
  ["present", [1, 2, 3], 2, true],
  ["absent", [1, 2, 3], 4, false],
  ["empty", [], 0, false],
  ["single-yes", [5], 5, true],
  ["single-no", [5], 0, false],
  ["first", [7, 8, 9], 7, true],
  ["last", [7, 8, 9], 9, true],
  ["duplicates", [2, 2, 2], 2, true],
  ["negative", [-1, -2, -3], -2, true],
  ["zero-absent", [-1, 1], 0, false],
  ["limit", [2147483647, -2147483648], -2147483648, true],
  ["limit-absent", [2147483647], -2147483648, false],
];

for (const [name, items, value, want] of cases) {
  test(name, () => {
    const items0 = [...items];
    assert.equal(contains(items, value), want);
    assert.deepEqual(items, items0);
  });
}
