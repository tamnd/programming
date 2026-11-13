// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-list/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { reverseList } from "./reverse_list.js";

const cases = [
  ["three", [1, 2, 3], [3, 2, 1]],
  ["four", [4, 7, 1, 9], [9, 1, 7, 4]],
  ["empty", [], []],
  ["single", [5], [5]],
  ["two", [1, 2], [2, 1]],
  ["duplicates", [1, 1, 2], [2, 1, 1]],
  ["palindrome", [1, 2, 1], [1, 2, 1]],
  ["negative", [-1, 0, -3], [-3, 0, -1]],
  ["limit", [2147483647, -2147483648, 0], [0, -2147483648, 2147483647]],
];

for (const [name, items, want] of cases) {
  test(name, () => {
    const items0 = [...items];
    const got = reverseList(items);
    assert.deepEqual(got, want);
    assert.notEqual(got, items);
    assert.deepEqual(items, items0);
  });
}
