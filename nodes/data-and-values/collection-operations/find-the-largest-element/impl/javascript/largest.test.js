// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-largest-element/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { largest } from "./largest.js";

const cases = [
  ["digits", [3, 1, 4, 1, 5], 5],
  ["middle", [10, 20, 15], 20],
  ["single", [7], 7],
  ["all-negative", [-3, -1, -2], -1],
  ["first", [9, 2, 3], 9],
  ["last", [1, 2, 9], 9],
  ["duplicates", [5, 9, 9, 1], 9],
  ["all-same", [4, 4, 4], 4],
  ["zero", [-5, 0, -1], 0],
  ["extremes", [-2147483648, 2147483647, 0], 2147483647],
  ["min-only", [-2147483648], -2147483648],
];

for (const [name, items, want] of cases) {
  test(name, () => {
    assert.deepEqual(largest(items), want);
  });
}

test("empty throws", () => {
  assert.throws(() => largest([]), RangeError);
});
