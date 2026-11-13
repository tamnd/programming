// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-list/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { insertAt } from "./insert_at.ts";

const cases: [string, number[], number, number, number[]][] = [
  ["at-end", [1, 2], 2, 3, [1, 2, 3]],
  ["at-front", [2, 3], 0, 1, [1, 2, 3]],
  ["in-middle", [1, 3], 1, 2, [1, 2, 3]],
  ["empty", [], 0, 7, [7]],
  ["single-front", [5], 0, 4, [4, 5]],
  ["single-end", [5], 1, 6, [5, 6]],
  ["end-of-many", [1, 2, 3, 4], 4, 5, [1, 2, 3, 4, 5]],
  ["front-of-many", [1, 2, 3, 4], 0, 0, [0, 1, 2, 3, 4]],
  ["duplicates", [1, 1], 1, 1, [1, 1, 1]],
  ["limit", [2147483647], 1, -2147483648, [2147483647, -2147483648]],
];

for (const [name, items, index, value, want] of cases) {
  test(name, () => {
    const items0 = [...items];
    assert.deepEqual(insertAt(items, index, value), want);
    assert.deepEqual(items, items0);
  });
}

test("index past the end", () => {
  assert.throws(() => insertAt([1, 2], 3, 9), RangeError);
});
