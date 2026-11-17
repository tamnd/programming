// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-generic-function/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { countGreater } from "./count-greater.js";

const cases = [
  ["some", [34, 50, 25, 100, 65], 50, 2],
  ["all", [3, 4, 5], 2, 3],
  ["none", [3, 4, 5], 9, 0],
  ["empty", [], 0, 0],
  ["equal-not-counted", [5, 5, 5], 5, 0],
  ["duplicates", [7, 1, 7, 7], 6, 3],
  ["negative", [-3, -1, 0, 2], -2, 3],
  ["min-limit", [-2147483648, 0, 2147483647], -2147483648, 2],
  ["max-limit", [2147483647, 1], 2147483647, 0],
  ["single", [1], 0, 1],
];

for (const [name, items, limit, want] of cases) {
  test(name, () => {
    assert.equal(countGreater(items, limit), want);
  });
}

test("strings", () => {
  assert.equal(countGreater(["pear", "apple", "plum"], "orange"), 2);
});
